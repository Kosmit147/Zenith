#include "Sandbox.hpp"

#include <glm/glm.hpp>

namespace {

const zth::ApplicationSpec app_spec = {
    .window_spec = {
        .width = 1920,
        .height = 1080,
        .title = "Sandbox",
        .gl_version = { 4, 6 },
        .gl_profile = zth::GlProfile::Core,
        .vsync = true,
    },
    .logger_spec = {
        .client_logger_label = "SANDBOX",
        .log_file_path = "log/log.txt",
    }
};

}

ZTH_IMPLEMENT_APP(Sandbox)

static constexpr auto vertex_shader_src = R"(
#version 460 core

layout (location = 0) in vec2 inPosition;
layout (location = 1) in vec4 inColor;

out vec4 Color;

void main()
{
    gl_Position = vec4(inPosition, 0.0, 1.0);
    Color = inColor;
}
)";

static constexpr auto fragment_shader_src = R"(
#version 460 core

in vec4 Color;

out vec4 outColor;

void main()
{
    outColor = Color;
}
)";

struct TriangleVertex
{
    glm::vec2 pos;
    glm::vec4 color;
};

Sandbox::Sandbox() : Application(app_spec)
{
    static constexpr std::array vertices = {
        TriangleVertex{ { -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
        TriangleVertex{ { 0.0f, 0.5f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
        TriangleVertex{ { 0.5f, -0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
    };

    GLuint va;
    glGenVertexArrays(1, &va);
    glBindVertexArray(va);

    GLuint vb;
    glGenBuffers(1, &vb);
    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TriangleVertex), vertices.data(), GL_STATIC_DRAW);

    // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), reinterpret_cast<const void*>(0));

    // color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex),
                          reinterpret_cast<const void*>(sizeof(TriangleVertex::pos)));

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);

    const auto* source = static_cast<const GLchar*>(vertex_shader_src);
    glShaderSource(vertex_shader, 1, &source, nullptr);
    glCompileShader(vertex_shader);

    GLint is_compiled = 0;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &is_compiled);

    if (is_compiled == GL_FALSE)
    {
        GLint max_length = 0;
        glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &max_length);

        std::vector<GLchar> info_log(max_length);
        glGetShaderInfoLog(vertex_shader, max_length, &max_length, &info_log[0]);

        glDeleteShader(vertex_shader);
        return;
    }

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    source = static_cast<const GLchar*>(fragment_shader_src);
    glShaderSource(fragment_shader, 1, &source, nullptr);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &is_compiled);

    if (is_compiled == GL_FALSE)
    {
        GLint max_length = 0;
        glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &max_length);

        std::vector<GLchar> info_log(max_length);
        glGetShaderInfoLog(fragment_shader, max_length, &max_length, info_log.data());

        glDeleteShader(fragment_shader);
        glDeleteShader(vertex_shader);
        return;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    glLinkProgram(program);

    GLint is_linked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &is_linked);

    if (is_linked == GL_FALSE)
    {
        GLint max_length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length);

        std::vector<GLchar> info_log(max_length);
        glGetProgramInfoLog(program, max_length, &max_length, info_log.data());

        glDeleteProgram(program);
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        return;
    }

    glDetachShader(program, vertex_shader);
    glDetachShader(program, fragment_shader);
    glUseProgram(program);

    _shader = program;
}

Sandbox::~Sandbox()
{
    glDeleteProgram(_shader);
}

auto Sandbox::on_update() -> void
{
    if (zth::Input::is_key_pressed(zth::Key::Space))
        ZTH_INFO("Space pressed!");

    glDrawArrays(GL_TRIANGLES, 0, 3);
}
