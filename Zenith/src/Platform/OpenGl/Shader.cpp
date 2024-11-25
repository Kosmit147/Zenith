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

// returns 0 if failed to create a shader
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

// returns 0 if failed to create a shader program
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

    auto uniform_name = std::make_unique<char[]>(max_unif_name_len);

    for (GLint i = 0; i < uniform_count; i++)
    {
        GLsizei unif_name_length;
        GLsizei unif_size;
        GLenum unif_type;

        glGetActiveUniform(_id, i, max_unif_name_len, &unif_name_length, &unif_size, &unif_type, uniform_name.get());

        auto uniform_info = UniformInfo{
            .location = glGetUniformLocation(_id, uniform_name.get()),
            .size = unif_size,
        };

        _uniform_map.emplace(std::string(uniform_name.get(), unif_name_length), uniform_info);
    }
}

auto Shader::get_unif_info(const std::string& name) const -> std::optional<UniformInfo>
{
    if (auto res = _uniform_map.find(name); res != _uniform_map.end())
    {
        auto& unif_info = res->second;
        return unif_info;
    }
    else
    {
        return {};
    }
}

auto Shader::set_unif(GLint location, GLint val) const -> void
{
    glUniform1i(location, val);
}

auto Shader::set_unif(GLint location, GLfloat val) const -> void
{
    glUniform1f(location, val);
}

auto Shader::set_unif(GLint location, glm::vec2 val) const -> void
{
    glUniform2f(location, val.x, val.y);
}

auto Shader::set_unif(GLint location, glm::vec3 val) const -> void
{
    glUniform3f(location, val.x, val.y, val.z);
}

auto Shader::set_unif(GLint location, glm::vec4 val) const -> void
{
    glUniform4f(location, val.x, val.y, val.z, val.w);
}

} // namespace zth
