#include "zenith/gl/shader.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/structured_bindings.hpp>

#include "zenith/core/assert.hpp"
#include "zenith/embedded/shaders.hpp"
#include "zenith/fs/fs.hpp"
#include "zenith/graphics/shader_preprocessor.hpp"
#include "zenith/util/defer.hpp"

namespace zth {

namespace gl {

// @test: all shader types (tess control, tess evaluation, geometry) and loading them from files

Shader::Shader(const ShaderSources& sources)
{
    auto program = create_program_from_sources(sources);

    if (!program)
    {
        ZTH_CORE_WARN("[Shader] Shader compilation failed. Switching to fallback shader.");

        program = create_fallback_program();

        if (!program)
            return;
    }

    _id = *program;
    retrieve_unif_info();
}

Shader::Shader(const ShaderSourcePaths& paths)
{
    auto program = create_program_from_files(paths);

    if (!program)
    {
        ZTH_CORE_WARN("[Shader] Shader compilation failed. Switching to fallback shader.");

        program = create_fallback_program();

        if (!program)
            return;
    }

    _id = *program;
    retrieve_unif_info();
}

auto Shader::from_sources(const ShaderSources& sources) -> Shader
{
    return Shader{ sources };
}

auto Shader::from_files(const ShaderSourcePaths& paths) -> Shader
{
    return Shader{ paths };
}

Shader::Shader(Shader&& other) noexcept : _id(other._id), _uniform_map(std::move(other._uniform_map))
{
    other._id = GL_NONE;
}

auto Shader::operator=(Shader&& other) noexcept -> Shader&
{
    _id = other._id;
    _uniform_map = std::move(other._uniform_map);

    other._id = GL_NONE;

    return *this;
}

Shader::~Shader()
{
    glDeleteProgram(_id);
}

auto Shader::retrieve_unif_info() -> void
{
    GLint uniform_count = 0;
    glGetProgramiv(_id, GL_ACTIVE_UNIFORMS, &uniform_count);

    if (uniform_count == 0)
        return;

    GLint max_unif_name_len = 0;
    glGetProgramiv(_id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_unif_name_len);

    auto uniform_name = std::make_unique<char[]>(static_cast<std::size_t>(max_unif_name_len));

    for (GLint i = 0; i < uniform_count; i++)
    {
        GLsizei unif_name_length;
        GLsizei unif_size;
        GLenum unif_type;

        glGetActiveUniform(_id, i, max_unif_name_len, &unif_name_length, &unif_size, &unif_type, uniform_name.get());

        UniformInfo uniform_info = {
            .location = glGetUniformLocation(_id, uniform_name.get()),
            .size = unif_size,
        };

        _uniform_map.emplace(uniform_name.get(), uniform_info);
    }
}

auto Shader::get_unif_info(std::string_view name) const -> std::optional<UniformInfo>
{
    if (auto kv = _uniform_map.find(name); kv != _uniform_map.end())
    {
        auto& [_, unif_info] = *kv;
        return unif_info;
    }
    else [[unlikely]]
    {
        return {};
    }
}

auto Shader::set_unif(GLint location, GLint val) -> void
{
    glUniform1i(location, val);
}

auto Shader::set_unif(GLint location, GLfloat val) -> void
{
    glUniform1f(location, val);
}

auto Shader::set_unif(GLint location, glm::vec2 val) -> void
{
    auto [x, y] = val;
    glUniform2f(location, x, y);
}

auto Shader::set_unif(GLint location, glm::vec3 val) -> void
{
    auto [x, y, z] = val;
    glUniform3f(location, x, y, z);
}

auto Shader::set_unif(GLint location, glm::vec4 val) -> void
{
    auto [x, y, z, w] = val;
    glUniform4f(location, x, y, z, w);
}

auto Shader::set_unif(GLint location, const glm::mat4& val) -> void
{
    glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(val));
}

auto Shader::create_shaders_from_sources(const ShaderSources& sources) -> std::optional<InPlaceVector<ShaderId, 5>>
{
    InPlaceVector<ShaderId, 5> shaders;
    Defer shaders_cleanup{ [&] { delete_shaders(shaders); } };

    auto add_shader = [&](std::string_view source, ShaderType type) {
        auto shader = create_shader(source, type);

        if (!shader)
            return false;

        shaders.push_back(*shader);
        return true;
    };

    if (!add_shader(sources.vertex_source, ShaderType::Vertex))
        return {};

    if (!add_shader(sources.fragment_source, ShaderType::Fragment))
        return {};

    if (sources.tess_control_source)
    {
        if (!add_shader(*sources.tess_control_source, ShaderType::TessControl))
            return {};
    }

    if (sources.tess_evaluation_source)
    {
        if (!add_shader(*sources.tess_evaluation_source, ShaderType::TessEvaluation))
            return {};
    }

    if (sources.geometry_source)
    {
        if (!add_shader(*sources.geometry_source, ShaderType::Geometry))
            return {};
    }

    shaders_cleanup.dismiss();
    return shaders;
}

auto Shader::create_shader(const std::string_view source, ShaderType type) -> std::optional<ShaderId>
{
    auto preprocessed_source = ShaderPreprocessor::preprocess(source);

    if (!preprocessed_source)
    {
        ZTH_CORE_ERROR("[Shader] Failed to preprocess shader source: {}", preprocessed_source.error());
        ZTH_DEBUG_BREAK;
        return {};
    }

    auto shader = glCreateShader(to_gl_enum(type));
    Defer delete_shader{ [&] { glDeleteShader(shader); } };

    const std::array sources = { preprocessed_source->data() };
    const std::array sources_lengths = { static_cast<GLint>(preprocessed_source->size()) };
    glShaderSource(shader, static_cast<GLsizei>(sources.size()), sources.data(), sources_lengths.data());

    if (!compile_shader(shader, type))
        return {};

    delete_shader.dismiss();
    return shader;
}

auto Shader::compile_shader(ShaderId id, ShaderType type) -> bool
{
    glCompileShader(id);

    GLint is_compiled = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &is_compiled);

    if (is_compiled == GL_FALSE)
    {
        GLint max_length = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &max_length);

        std::vector<GLchar> info_log(static_cast<std::size_t>(max_length));

        // glGetShaderInfoLog returns a null-terminated string
        glGetShaderInfoLog(id, max_length, &max_length, info_log.data());

        ZTH_CORE_ERROR("[Shader] Failed to compile {} shader: {}", type, info_log.data());
        ZTH_DEBUG_BREAK;
        return false;
    }

    return true;
}

auto Shader::create_program_from_sources(const ShaderSources& sources) -> std::optional<ProgramId>
{
    auto shaders = create_shaders_from_sources(sources);

    if (!shaders)
        return {};

    Defer shaders_cleanup{ [&] { delete_shaders(*shaders); } };

    auto program = create_program(*shaders);

    if (!program)
        return {};

#if !defined(ZTH_DIST_BUILD)
    // we're not deleting the shaders in non-distribution builds because that lets us look at their source code when
    // using api debugging tools
    shaders_cleanup.dismiss();
#endif

    return program;
}

auto Shader::create_program_from_files(const ShaderSourcePaths& paths) -> std::optional<ProgramId>
{
    auto vertex_source = fs::load_to_string(paths.vertex_file_path);
    auto fragment_source = fs::load_to_string(paths.fragment_file_path);

    // @robustness: .string() function on std::filesystem::filepath can throw

    if (!vertex_source)
    {
        ZTH_CORE_ERROR("[Shader] Failed to load vertex shader source from file {}.", paths.vertex_file_path.string());
        return {};
    }

    if (!fragment_source)
    {
        ZTH_CORE_ERROR("[Shader] Failed to load fragment shader source from file {}.",
                       paths.fragment_file_path.string());
        return {};
    }

    ShaderSources sources = {
        .vertex_source = *vertex_source,
        .fragment_source = *fragment_source,
    };

    // these variables cannot be moved to inner scope as we need them to live until the call to
    // create_shader_program_from_sources
    std::optional<std::string> tess_control_source;
    std::optional<std::string> tess_evaluation_source;
    std::optional<std::string> geometry_source;

    if (paths.tess_control_file_path)
    {
        tess_control_source = fs::load_to_string(*paths.tess_control_file_path);

        if (!tess_control_source)
        {
            ZTH_CORE_ERROR("[Shader] Failed to load tesselation control shader source from file {}.",
                           paths.tess_control_file_path->string());
            return {};
        }

        sources.tess_control_source = *tess_control_source;
    }

    if (paths.tess_evaluation_file_path)
    {
        tess_evaluation_source = fs::load_to_string(*paths.tess_evaluation_file_path);

        if (!tess_evaluation_source)
        {
            ZTH_CORE_ERROR("[Shader] Failed to load tesselation evaluation shader source from file {}.",
                           paths.tess_evaluation_file_path->string());
            return {};
        }

        sources.tess_evaluation_source = *tess_evaluation_source;
    }

    if (paths.geometry_file_path)
    {
        geometry_source = fs::load_to_string(*paths.geometry_file_path);

        if (!geometry_source)
        {
            ZTH_CORE_ERROR("[Shader] Failed to load geometry shader source from file {}.",
                           paths.geometry_file_path->string());
            return {};
        }

        sources.geometry_source = *geometry_source;
    }

    return create_program_from_sources(sources);
}

auto Shader::create_program(const InPlaceVector<ShaderId, 5>& shaders) -> std::optional<ProgramId>
{
    auto program = glCreateProgram();
    Defer delete_program{ [&] { glDeleteProgram(program); } };

    for (auto shader : shaders)
        glAttachShader(program, shader);

    auto success = link_program(program);

    for (auto shader : shaders)
        glDetachShader(program, shader);

    if (success)
    {
        delete_program.dismiss();
        return program;
    }
    else
    {
        return {};
    }
}

auto Shader::link_program(ProgramId id) -> bool
{
    glLinkProgram(id);

    GLint is_linked = 0;
    glGetProgramiv(id, GL_LINK_STATUS, &is_linked);

    if (is_linked == GL_FALSE)
    {
        GLint max_length = 0;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &max_length);

        std::vector<GLchar> info_log(static_cast<std::size_t>(max_length));

        // glGetProgramInfoLog returns a null-terminated string
        glGetProgramInfoLog(id, max_length, &max_length, info_log.data());

        ZTH_CORE_ERROR("[Shader] Failed to link shader: {}", info_log.data());
        ZTH_DEBUG_BREAK;
        return false;
    }

    return true;
}

auto Shader::delete_shaders(const InPlaceVector<ShaderId, 5>& shaders) -> void
{
    for (auto shader : shaders)
        glDeleteShader(shader);
}

auto Shader::create_fallback_program() -> std::optional<ProgramId>
{
    const ShaderSources fallback_program_sources = {
        .vertex_source = embedded::shaders::fallback_vert,
        .fragment_source = embedded::shaders::fallback_frag,
    };

    auto program = create_program_from_sources(fallback_program_sources);

    if (!program)
    {
        ZTH_CORE_CRITICAL("[Shader] Failed to compile fallback shader.");
        return {};
    }

    return program;
}

auto to_gl_enum(ShaderType shader_type) -> GLenum
{
    switch (shader_type)
    {
        using enum ShaderType;
    case Compute:
        return GL_COMPUTE_SHADER;
    case Vertex:
        return GL_VERTEX_SHADER;
    case TessControl:
        return GL_TESS_CONTROL_SHADER;
    case TessEvaluation:
        return GL_TESS_EVALUATION_SHADER;
    case Geometry:
        return GL_GEOMETRY_SHADER;
    case Fragment:
        return GL_FRAGMENT_SHADER;
    }

    ZTH_ASSERT(false);
    std::unreachable();
}

} // namespace gl

auto to_string(gl::ShaderType shader_type) -> const char*
{
    switch (shader_type)
    {
        using enum gl::ShaderType;
    case Compute:
        return "Compute";
    case Vertex:
        return "Vertex";
    case TessControl:
        return "Tesselation Control";
    case TessEvaluation:
        return "Tesselation Evaluation";
    case Geometry:
        return "Geometry";
    case Fragment:
        return "Fragment";
    }

    ZTH_ASSERT(false);
    return "Unknown";
}

} // namespace zth

ZTH_DEFINE_FORMATTER(zth::gl::ShaderType, type)
{
    ZTH_FORMAT_OUT("{}", zth::to_string(type));
}
