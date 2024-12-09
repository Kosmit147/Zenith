#include "TransformTest.hpp"

#include <battery/embed.hpp>
#include <imgui.h>

#include <algorithm>

namespace {

const auto container_texture = b::embed<"assets/container.jpg">().vec();
const auto emoji_texture = b::embed<"assets/emoji.png">().vec();
const auto wall_texture = b::embed<"assets/wall.jpg">().vec();

constexpr auto camera_position = glm::vec3(0.0f, 0.0f, 5.0f);
constexpr auto camera_front = glm::vec3(0.0f, 0.0f, -1.0f);

constexpr auto aspect_ratio = 16.0f / 9.0f;
constexpr auto fov = glm::radians(45.0f);

constexpr auto min_camera_pitch = glm::radians(-89.0f);
constexpr auto max_camera_pitch = glm::radians(89.0f);

constexpr auto camera_movement_speed = 1.5f;
constexpr auto camera_sprint_speed_multiplier = 3.0f;
constexpr auto camera_sensitivity = 0.001f;

} // namespace

TransformTest::TransformTest()
    : _cube_texture(container_texture), _cube_material(zth::shaders::texture_shader, &_cube_texture),
      _camera(std::make_shared<zth::PerspectiveCamera>(camera_position, camera_front, aspect_ratio, fov))
{}

auto TransformTest::on_load() -> void
{
    zth::Renderer::set_camera(_camera);
}

auto TransformTest::on_update() -> void
{
    render_ui();
    update_camera();

    _rotation_axis = glm::normalize(_rotation_axis);

    if (_uniform_scale)
        _scale = glm::vec3{ _scale.x };

    _cube.set_translation(_translation);
    _cube.set_rotation(_rotation_angle, _rotation_axis);
    _cube.set_scale(_scale);

    zth::Renderer::submit(_cube, _cube_material);
}

auto TransformTest::on_event(const zth::Event& event) -> void
{
    if (event.type() == zth::EventType::WindowResized)
    {
        auto window_resized_event = event.window_resized_event();
        on_window_resized_event(window_resized_event);
    }
}

auto TransformTest::on_window_resized_event(const zth::WindowResizedEvent& event) const -> void
{
    auto new_size = event.new_size;
    _camera->set_aspect_ratio(static_cast<float>(new_size.x) / static_cast<float>(new_size.y));
}

auto TransformTest::update_camera() const -> void
{
    if (zth::Window::cursor_enabled())
        return;

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

auto TransformTest::render_ui() -> void
{
    ImGui::Begin("Transform");

    constexpr auto drag_speed = 0.01f;

    ImGui::DragFloat3("Translation", reinterpret_cast<float*>(&_translation), drag_speed);
    ImGui::DragFloat("Rotation Angle", &_rotation_angle, drag_speed);
    ImGui::DragFloat3("Rotation Axis", reinterpret_cast<float*>(&_rotation_axis), drag_speed);

    if (_uniform_scale)
        ImGui::DragFloat("Scale", reinterpret_cast<float*>(&_scale), drag_speed);
    else
        ImGui::DragFloat3("Scale", reinterpret_cast<float*>(&_scale), drag_speed);

    ImGui::Checkbox("Uniform Scale", &_uniform_scale);

    ImGui::End();
}
