#include "zenith/gl/shader.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/structured_bindings.hpp>

#include "zenith/core/assert.hpp"
#include "zenith/embedded/shaders.hpp"
#include "zenith/fs/file.hpp"
#include "zenith/memory/temporary_storage.hpp"
#include "zenith/renderer/shader_preprocessor.hpp"
#include "zenith/util/defer.hpp"

namespace zth {

namespace gl {

// @test: All shader types (tess control, tess evaluation, geometry) and loading them from files.

Shader::Shader(const ShaderSources& sources)
{
    auto program = create_program_from_sources(sources);

    if (!program)
    {
        ZTH_INTERNAL_WARN("[Shader] Shader compilation failed. Switching to fallback shader.");

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
        ZTH_INTERNAL_WARN("[Shader] Shader compilation failed. Switching to fallback shader.");

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

Shader::Shader(Shader&& other) noexcept
    : _id(std::exchange(other._id, GL_NONE)), _uniform_map(std::move(other._uniform_map))
{}

auto Shader::operator=(Shader&& other) noexcept -> Shader&
{
    _id = std::exchange(other._id, GL_NONE);
    _uniform_map = std::move(other._uniform_map);
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

    // @speed: Use temporary storage.
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

auto Shader::get_unif_info(StringView name) const -> Optional<UniformInfo>
{
    if (auto kv = _uniform_map.find(name); kv != _uniform_map.end())
    {
        auto& [_, unif_info] = *kv;
        return unif_info;
    }
    else [[unlikely]]
    {
        return nil;
    }
}

auto Shader::get_unif_location(StringView name) const -> Optional<GLint>
{
    return get_unif_info(name).transform([](auto unif_info) { return unif_info.location; });
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
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(val));
}

auto Shader::create_shaders_from_sources(const ShaderSources& sources) -> Optional<InPlaceVector<ShaderId, 5>>
{
    InPlaceVector<ShaderId, 5> shaders;
    Defer shaders_cleanup{ [&] { delete_shaders(shaders); } };

    auto add_shader = [&](StringView source, ShaderType type) {
        auto shader = create_shader(source, type);

        if (!shader)
            return false;

        shaders.push_back(*shader);
        return true;
    };

    if (!add_shader(sources.vertex_source, ShaderType::Vertex))
        return nil;

    if (!add_shader(sources.fragment_source, ShaderType::Fragment))
        return nil;

    if (sources.tess_control_source)
    {
        if (!add_shader(*sources.tess_control_source, ShaderType::TessControl))
            return nil;
    }

    if (sources.tess_evaluation_source)
    {
        if (!add_shader(*sources.tess_evaluation_source, ShaderType::TessEvaluation))
            return nil;
    }

    if (sources.geometry_source)
    {
        if (!add_shader(*sources.geometry_source, ShaderType::Geometry))
            return nil;
    }

    shaders_cleanup.dismiss();
    return shaders;
}

auto Shader::create_shader(const StringView source, ShaderType type) -> Optional<ShaderId>
{
    auto preprocessed_source = ShaderPreprocessor::preprocess(source);

    if (!preprocessed_source)
    {
        ZTH_INTERNAL_ERROR("[Shader] Failed to preprocess shader source: {}", preprocessed_source.error());
        ZTH_DEBUG_BREAK;
        return nil;
    }

    auto shader = glCreateShader(to_gl_enum(type));
    Defer delete_shader{ [&] { glDeleteShader(shader); } };

    const std::array sources = { preprocessed_source->data() };
    const std::array sources_lengths = { static_cast<GLint>(preprocessed_source->size()) };
    glShaderSource(shader, static_cast<GLsizei>(sources.size()), sources.data(), sources_lengths.data());

    if (!compile_shader(shader, type))
        return nil;

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

        TemporaryVector<GLchar> info_log(max_length);

        // glGetShaderInfoLog returns a null-terminated string.
        glGetShaderInfoLog(id, max_length, &max_length, info_log.data());

        ZTH_INTERNAL_ERROR("[Shader] Failed to compile {} shader: {}", type, info_log.data());
        ZTH_DEBUG_BREAK;
        return false;
    }

    return true;
}

auto Shader::create_program_from_sources(const ShaderSources& sources) -> Optional<ProgramId>
{
    auto shaders = create_shaders_from_sources(sources);

    if (!shaders)
        return nil;

    Defer shaders_cleanup{ [&] { delete_shaders(*shaders); } };

    auto program = create_program(*shaders);

    if (!program)
        return nil;

#if !defined(ZTH_DIST_BUILD)
    // We're not deleting the shaders in non-distribution builds because that lets us look at their source code when
    // using API debugging tools.
    shaders_cleanup.dismiss();
#endif

    return program;
}

auto Shader::create_program_from_files(const ShaderSourcePaths& paths) -> Optional<ProgramId>
{
    auto vertex_source = fs::load_to<TemporaryString>(paths.vertex_path);
    auto fragment_source = fs::load_to<TemporaryString>(paths.fragment_path);

    // @robustness: .string() throws.

    if (!vertex_source)
    {
        ZTH_INTERNAL_ERROR("[Shader] Failed to load vertex shader source from file {}.", paths.vertex_path.string());
        return nil;
    }

    if (!fragment_source)
    {
        ZTH_INTERNAL_ERROR("[Shader] Failed to load fragment shader source from file {}.",
                           paths.fragment_path.string());
        return nil;
    }

    ShaderSources sources = {
        .vertex_source = *vertex_source,
        .fragment_source = *fragment_source,
    };

    // These variables cannot be moved to inner scope as we need them to live until the call to
    // create_shader_program_from_sources.
    Optional<TemporaryString> tess_control_source;
    Optional<TemporaryString> tess_evaluation_source;
    Optional<TemporaryString> geometry_source;

    if (paths.tess_control_path)
    {
        tess_control_source = fs::load_to<TemporaryString>(*paths.tess_control_path);

        if (!tess_control_source)
        {
            ZTH_INTERNAL_ERROR("[Shader] Failed to load tesselation control shader source from file {}.",
                               paths.tess_control_path->string());
            return nil;
        }

        sources.tess_control_source = *tess_control_source;
    }

    if (paths.tess_evaluation_path)
    {
        tess_evaluation_source = fs::load_to<TemporaryString>(*paths.tess_evaluation_path);

        if (!tess_evaluation_source)
        {
            ZTH_INTERNAL_ERROR("[Shader] Failed to load tesselation evaluation shader source from file {}.",
                               paths.tess_evaluation_path->string());
            return nil;
        }

        sources.tess_evaluation_source = *tess_evaluation_source;
    }

    if (paths.geometry_path)
    {
        geometry_source = fs::load_to<TemporaryString>(*paths.geometry_path);

        if (!geometry_source)
        {
            ZTH_INTERNAL_ERROR("[Shader] Failed to load geometry shader source from file {}.",
                               paths.geometry_path->string());
            return nil;
        }

        sources.geometry_source = *geometry_source;
    }

    return create_program_from_sources(sources);
}

auto Shader::create_program(const InPlaceVector<ShaderId, 5>& shaders) -> Optional<ProgramId>
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
        return nil;
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

        TemporaryVector<GLchar> info_log(max_length);

        // glGetProgramInfoLog returns a null-terminated string.
        glGetProgramInfoLog(id, max_length, &max_length, info_log.data());

        ZTH_INTERNAL_ERROR("[Shader] Failed to link shader: {}", info_log.data());
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

auto Shader::create_fallback_program() -> Optional<ProgramId>
{
    const ShaderSources fallback_program_sources = {
        .vertex_source = embedded::shaders::fallback_vert,
        .fragment_source = embedded::shaders::fallback_frag,
    };

    auto program = create_program_from_sources(fallback_program_sources);

    if (!program)
    {
        ZTH_INTERNAL_CRITICAL("[Shader] Failed to compile fallback shader.");
        return nil;
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
        return "TessControl";
    case TessEvaluation:
        return "TessEvaluation";
    case Geometry:
        return "Geometry";
    case Fragment:
        return "Fragment";
    }

    ZTH_ASSERT(false);
    return "Unknown";
}

} // namespace zth

ZTH_DEFINE_FORMATTER(zth::gl::ShaderType, shader_type)
{
    return ZTH_FORMAT_OUT("{}", zth::to_string(shader_type));
}

ZTH_DEFINE_FORMATTER(zth::gl::ShaderSourcePaths, paths)
{
    // @test: This function.
    // @robustness: .string() throws.

    ZTH_FORMAT_OUT("ShaderSourcePaths {{\n"
                   "\t.vertex_path = \"{}\",\n"
                   "\t.fragment_path = \"{}\",\n",
                   paths.vertex_path.string(), paths.fragment_path.string());

    {
        ZTH_FORMAT_OUT("\t.tess_control_path = ");

        if (paths.tess_control_path)
            ZTH_FORMAT_OUT("\"{}\",\n", paths.tess_control_path->string());
        else
            ZTH_FORMAT_OUT("nil,\n");
    }

    {
        ZTH_FORMAT_OUT("\t.tess_evaluation_path = ");

        if (paths.tess_evaluation_path)
            ZTH_FORMAT_OUT("\"{}\",\n", paths.tess_evaluation_path->string());
        else
            ZTH_FORMAT_OUT("nil,\n");
    }

    {
        ZTH_FORMAT_OUT("\t.geometry_path = ");

        if (paths.geometry_path)
            ZTH_FORMAT_OUT("\"{}\",\n", paths.geometry_path->string());
        else
            ZTH_FORMAT_OUT("nil,\n");
    }

    return ZTH_FORMAT_OUT("}}");
}
