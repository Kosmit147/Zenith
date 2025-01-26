#include "zenith/platform/gl/shader.hpp"

#include <battery/embed.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/structured_bindings.hpp>

#include "zenith/logging/logger.hpp"
#include "zenith/platform/shader_preprocessor.hpp"
#include "zenith/utility/cleanup.hpp"

namespace zth {

namespace {

// @cleanup: move to some kind of embedded module
const auto fallback_vertex_source = std::string_view(b::embed<"src/graphics/shaders/zth_fallback.vert">().data());
const auto fallback_fragment_source = std::string_view(b::embed<"src/graphics/shaders/zth_fallback.frag">().data());

[[nodiscard]] auto compile_shader(GLuint id, ShaderType type) -> bool
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

        ZTH_CORE_ERROR("Failed to compile {} shader: {}", type, info_log.data());
        ZTH_DEBUG_BREAK;
        return false;
    }

    return true;
}

// returns 0 if failed to create a shader
[[nodiscard]] auto create_shader(std::string_view source, ShaderType type) -> GLuint
{
    auto shader = glCreateShader(to_gl_enum(type));
    Cleanup shader_cleanup{ [&] { glDeleteShader(shader); } };

    const std::array sources = { source.data(), "\n" };
    glShaderSource(shader, static_cast<GLsizei>(sources.size()), sources.data(), nullptr);

    auto success = compile_shader(shader, type);

    if (success)
    {
        shader_cleanup.dismiss();
        return shader;
    }
    else
    {
        return GL_NONE;
    }
}

[[nodiscard]] auto link_shader_program(GLuint id) -> bool
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

        ZTH_CORE_ERROR("Failed to link shader: {}", info_log.data());
        ZTH_DEBUG_BREAK;
        return false;
    }

    return true;
}

// returns 0 if failed to create a shader program
[[nodiscard]] auto create_shader_program(GLuint vertex_shader, GLuint fragment_shader) -> GLuint
{
    auto program = glCreateProgram();
    Cleanup program_cleanup{ [&] { glDeleteProgram(program); } };

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    auto success = link_shader_program(program);

    glDetachShader(program, vertex_shader);
    glDetachShader(program, fragment_shader);

    if (success)
    {
        program_cleanup.dismiss();
        return program;
    }
    else
    {
        return GL_NONE;
    }
}

} // namespace

Shader::Shader(std::string_view vertex_source, std::string_view fragment_source)
{
    // @todo: log errors
    auto preprocessed_vertex_source = ShaderPreprocessor::preprocess(vertex_source);
    auto preprocessed_fragment_source = ShaderPreprocessor::preprocess(fragment_source);

    if (!preprocessed_vertex_source || !preprocessed_fragment_source)
        return;

    auto vertex_shader = create_shader(*preprocessed_vertex_source, ShaderType::Vertex);

    if (!vertex_shader)
        return;

    Cleanup vertex_shader_cleanup{ [&] { glDeleteShader(vertex_shader); } };

    auto fragment_shader = create_shader(*preprocessed_fragment_source, ShaderType::Fragment);

    if (!fragment_shader)
        return;

    Cleanup fragment_shader_cleanup{ [&] { glDeleteShader(fragment_shader); } };

    auto program = create_shader_program(vertex_shader, fragment_shader);

    if (!program)
        return;

#ifndef ZTH_DIST_BUILD
    vertex_shader_cleanup.dismiss();
    fragment_shader_cleanup.dismiss();
#endif

    _id = program;
    retrieve_unif_info();
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
    if (auto res = _uniform_map.find(name); res != _uniform_map.end())
    {
        auto& unif_info = res->second;
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

} // namespace zth
