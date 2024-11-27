#include "Sandbox.hpp"

#include <algorithm>

#include <battery/embed.hpp>

ZTH_IMPLEMENT_APP(Sandbox)

namespace {

const zth::ApplicationSpec app_spec = {
    .window_spec = {
        .width = 2560,
        .height = 1600,
        .title = "Sandbox",
        .gl_version = { 4, 6 },
        .gl_profile = zth::GlProfile::Core,
        .fullscreen = true,
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
    Vertex{ { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.5f, 1.0f } },
    Vertex{ { 0.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
};

constexpr std::array<GLushort, 3> indices = { 0, 1, 2 };

constexpr auto camera_position = glm::vec3(0.0f, 0.0f, 3.0f);
constexpr auto camera_front = glm::vec3(0.0f, 0.0f, -1.0f);

// TODO: fix hardcoded aspect ratio
const auto aspect_ratio =
    static_cast<float>(app_spec.window_spec.width) / static_cast<float>(app_spec.window_spec.height);
constexpr auto fov = glm::radians(45.0f);

constexpr auto min_camera_pitch = glm::radians(-89.0f);
constexpr auto max_camera_pitch = glm::radians(89.0f);

constexpr auto camera_movement_speed = 1.5f;
constexpr auto camera_sensitivity = 0.001f;

} // namespace

Sandbox::Sandbox()
    : Application(app_spec), _vb(vertices, zth::BufferUsage::static_draw), _ib(indices, zth::BufferUsage::static_draw),
      _shader(vertex_shader_source, fragment_shader_source), _texture(wall_texture),
      _camera(camera_position, camera_front, aspect_ratio, fov)
{
    _va.bind();
    _va.bind_vertex_buffer(_vb);
    _va.bind_index_buffer(_ib);
    _va.bind_layout(zth::VertexLayout::from_vertex<Vertex>());
}

auto Sandbox::on_update() -> void
{
    auto& engine = zth::Engine::get();
    auto time = static_cast<float>(engine.time());

    update_camera();

    constexpr auto model = glm::mat4(1.0f);
    const auto view = _camera.view();
    const auto projection = _camera.projection();

    _va.bind();
    _texture.bind();
    _shader.bind();
    _shader.set_unif("time", time);
    _shader.set_unif("tex", 0);
    _shader.set_unif("model", model);
    _shader.set_unif("view", view);
    _shader.set_unif("projection", projection);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, nullptr);
}

auto Sandbox::update_camera() -> void
{
    auto delta_time = static_cast<float>(zth::Engine::get().delta_time());

    if (zth::Input::is_key_pressed(zth::Key::W))
        _camera.position += _camera.front() * camera_movement_speed * delta_time;

    if (zth::Input::is_key_pressed(zth::Key::S))
        _camera.position -= _camera.front() * camera_movement_speed * delta_time;

    if (zth::Input::is_key_pressed(zth::Key::A))
        _camera.position -= _camera.right() * camera_movement_speed * delta_time;

    if (zth::Input::is_key_pressed(zth::Key::D))
        _camera.position += _camera.right() * camera_movement_speed * delta_time;

    auto mouse_delta = zth::Input::get_mouse_pos_delta();
    mouse_delta *= camera_sensitivity;

    auto new_yaw = _camera.yaw() + mouse_delta.x;
    auto new_pitch = std::clamp(_camera.pitch() - mouse_delta.y, min_camera_pitch, max_camera_pitch);

    _camera.set_yaw_and_pitch(new_yaw, new_pitch);
}
