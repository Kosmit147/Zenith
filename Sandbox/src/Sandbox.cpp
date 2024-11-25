#include "Sandbox.hpp"

#include <battery/embed.hpp>

ZTH_IMPLEMENT_APP(Sandbox)

namespace {

const zth::ApplicationSpec app_spec = {
    .window_spec = {
        .width = 1080,
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

const auto vertex_shader_source = b::embed<"src/shaders/test.vert">().str();
const auto fragment_shader_source = b::embed<"src/shaders/test.frag">().str();
const auto container_texture = b::embed<"assets/container.jpg">().vec();
const auto emoji_texture = b::embed<"assets/emoji.png">().vec();
const auto wall_texture = b::embed<"assets/wall.jpg">().vec();

constexpr std::array vertices = {
    Vertex{ { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
    Vertex{ { 0.0f, 1.0f, 0.0f, 1.0f },{ 0.5f, 1.0f } },
    Vertex{ { 0.0f, 0.0f, 1.0f, 1.0f },{ 1.0f, 0.0f } },
};

constexpr std::array<GLushort, 3> indices = { 0, 1, 2 };

} // namespace

Sandbox::Sandbox()
    : Application(app_spec), _vb(vertices, zth::BufferUsage::static_draw), _ib(indices, zth::BufferUsage::static_draw),
      _shader(vertex_shader_source, fragment_shader_source), _texture(wall_texture)
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

    auto time = static_cast<float>(glfwGetTime());

    _va.bind();
    _texture.bind();
    _shader.bind();
    _shader.set_unif("time", time);
    _shader.set_unif("tex", 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
}
