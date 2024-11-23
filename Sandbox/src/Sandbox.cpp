#include "Sandbox.hpp"

#include <battery/embed.hpp>

ZTH_IMPLEMENT_APP(Sandbox)

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

const auto vertex_shader_source = b::embed<"src/shaders/rect.vert">().str();
const auto fragment_shader_source = b::embed<"src/shaders/rect.frag">().str();

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
      _shader(vertex_shader_source, fragment_shader_source)
{
    _va.bind();
    _va.bind_vertex_buffer(_vb);
    _va.bind_index_buffer(_ib);
    _va.bind_layout(zth::VertexLayout::from_vertex<Vertex>());
}

auto Sandbox::on_update() -> void
{
    if (zth::Input::is_key_pressed(zth::Key::Space))
        ZTH_INFO("Space pressed!");

    _va.bind();
    _shader.bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
}
