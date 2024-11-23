#include "Sandbox.hpp"

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

namespace {

constexpr std::array vertices = {
    Vertex{ { -0.5f, 0.5f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
    Vertex{ { 0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
    Vertex{ { 0.5f, -0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
    Vertex{ { -0.5f, -0.5f }, { 0.2f, 0.9f, 0.5f, 1.0f } },
};

constexpr std::array<GLushort, 6> indices = {
    0, 1, 2, 2, 3, 0,
};

} // namespace

Sandbox::Sandbox()
    : Application(app_spec), _vb(vertices, zth::BufferUsage::static_draw), _ib(indices, zth::BufferUsage::static_draw),
      _shader(vertex_shader_src, fragment_shader_src)
{
    _va.bind();
    // with DSA
    // _va.bind_vertex_buffer(_vb);
    // _va.bind_index_buffer(_ib);

    _vb.bind();
    _ib.bind();

    // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(0));

    // color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(sizeof(Vertex::pos)));
}

auto Sandbox::on_update() -> void
{
    if (zth::Input::is_key_pressed(zth::Key::Space))
        ZTH_INFO("Space pressed!");

    _va.bind();
    _shader.bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
}
