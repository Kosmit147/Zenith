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

Sandbox::Sandbox() : Application(app_spec), _shader(vertex_shader_src, fragment_shader_src)
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
}

auto Sandbox::on_update() -> void
{
    if (zth::Input::is_key_pressed(zth::Key::Space))
        ZTH_INFO("Space pressed!");

    _shader.bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
