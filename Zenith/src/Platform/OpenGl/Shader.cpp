#include "Zenith/Platform/OpenGl/Shader.hpp"

#include "Zenith/Logging/Logger.hpp"

namespace zth {

namespace {

auto compile_shader(GLuint id, ShaderType type) -> bool
{
    glCompileShader(id);

    GLint is_compiled = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &is_compiled);

    if (is_compiled == GL_FALSE)
    {
        GLint max_length = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &max_length);

        std::vector<GLchar> info_log(max_length);

        // glGetShaderInfoLog returns a null-terminated string
        glGetShaderInfoLog(id, max_length, &max_length, info_log.data());

        ZTH_CORE_ERROR("Failed to compile {} shader: {}", to_string(type), info_log.data());
        ZTH_DEBUG_BREAK;
        return false;
    }

    return true;
}

auto create_shader(std::string_view source, ShaderType type) -> GLuint
{
    auto shader = glCreateShader(to_gl_enum(type));
    const char* sources[] = { source.data(), "\n\0" };
    glShaderSource(shader, 2, sources, nullptr);

    auto success = compile_shader(shader, type);

    if (success)
    {
        return shader;
    }
    else
    {
        glDeleteShader(shader);
        return 0;
    }
}

auto link_shader_program(GLuint id) -> bool
{
    glLinkProgram(id);

    GLint is_linked = 0;
    glGetProgramiv(id, GL_LINK_STATUS, &is_linked);

    if (is_linked == GL_FALSE)
    {
        GLint max_length = 0;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &max_length);

        std::vector<GLchar> info_log(max_length);

        // glGetProgramInfoLog returns a null-terminated string
        glGetProgramInfoLog(id, max_length, &max_length, info_log.data());

        ZTH_CORE_ERROR("Failed to link shader: {}", info_log.data());
        ZTH_DEBUG_BREAK;
        return false;
    }

    return true;
}

auto create_shader_program(GLuint vertex_shader, GLuint fragment_shader) -> GLuint
{
    auto program = glCreateProgram();

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    auto success = link_shader_program(program);

    glDetachShader(program, vertex_shader);
    glDetachShader(program, fragment_shader);

    if (success)
    {
        return program;
    }
    else
    {
        glDeleteProgram(program);
        return 0;
    }
}

} // namespace

Shader::Shader(std::string_view vertex_source, std::string_view fragment_source)
{
    auto vertex_shader = create_shader(vertex_source, ShaderType::Vertex);

    if (!vertex_shader)
        return;

    auto fragment_shader = create_shader(fragment_source, ShaderType::Fragment);

    if (!fragment_shader)
    {
        glDeleteShader(vertex_shader);
        return;
    }

    auto program = create_shader_program(vertex_shader, fragment_shader);

    if (!program)
    {
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        return;
    }

#ifndef _DEBUG
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
#endif

    _id = program;
}

Shader::~Shader()
{
    if (_id != 0)
        glDeleteProgram(_id);
}

} // namespace zth
