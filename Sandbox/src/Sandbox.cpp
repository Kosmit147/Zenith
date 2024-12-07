#include "Sandbox.hpp"

#include <battery/embed.hpp>
#include <glm/glm.hpp>

#include <algorithm>

ZTH_IMPLEMENT_APP(Sandbox)

namespace {

const zth::ApplicationSpec app_spec = {
    .window_spec = {
        .size = { .width = 800, .height = 600 },
        .title = "Sandbox",
        .gl_version = { 4, 6 },
        .gl_profile = zth::GlProfile::Core,
        .fullscreen = false,
        .vsync = true,
        .resizable = true,
        .maximized = true,
        .cursor_enabled = false,
        .transparent_framebuffer = false,
        .forced_aspect_ratio = std::nullopt,
    },
    .logger_spec = {
        .client_logger_label = "SANDBOX",
        .log_file_path = "log/log.txt",
    }
};

const auto container_texture = b::embed<"assets/container.jpg">().vec();
const auto emoji_texture = b::embed<"assets/emoji.png">().vec();
const auto wall_texture = b::embed<"assets/wall.jpg">().vec();

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

} // namespace

Sandbox::Sandbox()
    : Application(app_spec), _texture(wall_texture),
      _camera(std::make_shared<zth::PerspectiveCamera>(camera_position, camera_front, aspect_ratio, fov))
{
    zth::Renderer::set_camera(_camera);
}

auto Sandbox::on_update() -> void
{
    const auto time = zth::Time::time<float>();

    update_camera();

    _cube.rotate(0.0005f * time, glm::vec3{ 0.0f, 1.0f, 0.0f });
    _cube.rotate(0.0005f * time, glm::normalize(glm::vec3{ -0.3f, 0.1f, 0.7f }));

    // TODO: handle drawing cubes better
    zth::Renderer::submit(_cube, { &_cube.transform(), zth::shaders::texture_shader, &_texture });

    if (zth::Input::is_key_pressed(zth::Key::Escape))
        zth::Window::close();
}

auto Sandbox::on_event(const zth::Event& event) -> void
{
    switch (event.type())
    {
        using enum zth::EventType;
    case WindowResized:
    {
        auto window_resized_event = event.window_resized_event();
        ZTH_INFO("Window resized. New size: {}.", window_resized_event.new_size);
        on_window_resized_event(window_resized_event);
        break;
    }
    case KeyPressed:
    {
        auto [key] = event.key_pressed_event();
        ZTH_INFO("{} key pressed.", key);
        break;
    }
    case KeyReleased:
    {
        auto [key] = event.key_released_event();
        ZTH_INFO("{} key released.", key);
        break;
    }
    case MouseButtonPressed:
    {
        auto [button] = event.mouse_button_pressed_event();
        ZTH_INFO("{} mouse button pressed.", button);
        break;
    }
    case MouseButtonReleased:
    {
        auto [button] = event.mouse_button_released_event();
        ZTH_INFO("{} mouse button released.", button);
        break;
    }
    case MouseMoved:
    {
        auto [new_pos] = event.mouse_moved_event();
        ZTH_INFO("Mouse moved. New pos: {}.", new_pos);
        break;
    }
    case MouseWheelScrolled:
    {
        auto [delta] = event.mouse_wheel_scrolled_event();
        ZTH_INFO("Mouse scrolled. Delta: {}.", delta);
        break;
    }
    }
}

auto Sandbox::on_window_resized_event(const zth::WindowResizedEvent& event) const -> void
{
    auto [width, height] = event.new_size;
    _camera->set_aspect_ratio(static_cast<float>(width) / static_cast<float>(height));
}

auto Sandbox::update_camera() const -> void
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
