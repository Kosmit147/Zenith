#include "CubeGame.hpp"

#include <battery/embed.hpp>
#include <glm/glm.hpp>

#include <algorithm>

ZTH_IMPLEMENT_APP(CubeGame)

namespace {

const zth::ApplicationSpec app_spec = {
    .window_spec = {
        .size = { .width = 800, .height = 600 },
        .title = "Cube Game",
        .vsync = true,
        .maximized = true,
        .cursor_enabled = false,
    },
    .logger_spec = {
        .client_logger_label = "CUBE GAME",
        .log_file_path = "log/log.txt",
    }
};

const auto cobble_texture_data = b::embed<"assets/cobble.png">().vec();

constexpr auto camera_position = glm::vec3(0.0f, 0.0f, 5.0f);
constexpr auto camera_front = glm::vec3(0.0f, 0.0f, -1.0f);

const auto aspect_ratio =
    static_cast<float>(app_spec.window_spec.size.width) / static_cast<float>(app_spec.window_spec.size.height);

constexpr auto fov = glm::radians(45.0f);

constexpr auto min_camera_pitch = glm::radians(-89.0f);
constexpr auto max_camera_pitch = glm::radians(89.0f);

constexpr auto camera_movement_speed = 1.5f;
constexpr auto camera_sprint_speed_multiplier = 3.0f;
constexpr auto camera_sensitivity = 0.001f;

const zth::TextureParams cobble_texture_params = {
    .min_filter = zth::TextureMinFilter::nearest_mipmap_linear,
    .mag_filter = zth::TextureMagFilter::nearest,
};

} // namespace

CubeGame::CubeGame()
    : Application(app_spec), _cobble_texture(cobble_texture_data, cobble_texture_params),
      _cobble_material(zth::shaders::texture_shader, &_cobble_texture),
      _camera(std::make_shared<zth::PerspectiveCamera>(camera_position, camera_front, aspect_ratio, fov))
{
    zth::Renderer::set_camera(_camera);
}

auto CubeGame::on_update() -> void
{
    update_camera();

    auto rotation_speed = 0.0005f;

    _cube.rotate(rotation_speed, glm::normalize(glm::vec3{ 0.0f, 1.0f, 0.0f }));
    _cube.rotate(rotation_speed, glm::normalize(glm::vec3{ -0.3f, 0.1f, 0.7f }));
    _cube.rotate(rotation_speed, glm::normalize(glm::vec3{ 0.9f, 0.4f, 0.4f }));

    zth::Renderer::submit(_cube, _cobble_material);

    if (zth::Input::is_key_pressed(zth::Key::Escape))
        zth::Window::close();
}

auto CubeGame::on_event(const zth::Event& event) -> void
{
    if (event.type() == zth::EventType::WindowResized)
    {
        auto [new_size] = event.window_resized_event();
        auto [width, height] = new_size;
        _camera->set_aspect_ratio(static_cast<float>(width) / static_cast<float>(height));
    }
}

auto CubeGame::update_camera() const -> void
{
    auto delta_time = zth::Time::delta_time<float>();

    auto movement_speed = camera_movement_speed;

    if (zth::Input::is_key_pressed(zth::Key::LeftShift))
        movement_speed *= camera_sprint_speed_multiplier;

    auto new_camera_pos = _camera->position();

    if (zth::Input::is_key_pressed(zth::Key::W))
        new_camera_pos += _camera->front() * movement_speed * delta_time;

    if (zth::Input::is_key_pressed(zth::Key::S))
        new_camera_pos -= _camera->front() * movement_speed * delta_time;

    if (zth::Input::is_key_pressed(zth::Key::A))
        new_camera_pos -= _camera->right() * movement_speed * delta_time;

    if (zth::Input::is_key_pressed(zth::Key::D))
        new_camera_pos += _camera->right() * movement_speed * delta_time;

    _camera->set_position(new_camera_pos);

    auto mouse_delta = zth::Input::mouse_pos_delta();
    mouse_delta *= camera_sensitivity;

    auto new_yaw = _camera->yaw() + mouse_delta.x;
    auto new_pitch = std::clamp(_camera->pitch() - mouse_delta.y, min_camera_pitch, max_camera_pitch);

    _camera->set_yaw_and_pitch(new_yaw, new_pitch);
}
