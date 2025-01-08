#include "ContainersScene.hpp"

#include <imgui.h>

#include <ranges>

#include "Embedded.hpp"

namespace {

constexpr auto camera_position = glm::vec3{ 0.0f, 0.0f, 5.0f };
constexpr auto camera_front = glm::vec3{ 0.0f, 0.0f, -1.0f };

constexpr auto aspect_ratio = 16.0f / 9.0f;
constexpr auto fov = glm::radians(45.0f);

const auto directional_light_direction = glm::normalize(glm::vec3{ 0.0f, -1.0f, -1.0f });
constexpr auto point_light_position = glm::vec3{ -0.7f, 1.3f, 1.7f };

constexpr std::array container_positions = { glm::vec3{ 0.0f, 0.0f, 0.0f },    glm::vec3{ 2.0f, 5.0f, -15.0f },
                                             glm::vec3{ -1.5f, -2.2f, -2.5f }, glm::vec3{ -3.8f, -2.0f, -12.3f },
                                             glm::vec3{ 2.4f, -0.4f, -3.5f },  glm::vec3{ -1.7f, 3.0f, -7.5f },
                                             glm::vec3{ 1.3f, -2.0f, -2.5f },  glm::vec3{ 1.5f, 2.0f, -2.5f },
                                             glm::vec3{ 1.5f, 0.2f, -1.5f },   glm::vec3{ -1.3f, 1.0f, -1.5f } };

} // namespace

ContainersScene::ContainersScene()
    : _diffuse_map{ embedded::container2_diffuse_map_data }, _specular_map{ embedded::container2_specular_map_data },
      _container_material{ .diffuse_map = &_diffuse_map, .specular_map = &_specular_map },
      _light_marker_material{ .shader = &zth::shaders::flat_color() },
      _camera(std::make_shared<zth::PerspectiveCamera>(camera_position, camera_front, aspect_ratio, fov)),
      _camera_controller{ _camera },
      _directional_light(std::make_shared<zth::DirectionalLight>(directional_light_direction)),
      _point_light(std::make_shared<zth::PointLight>(point_light_position))
{
    _light_marker.set_translation(_point_light->translation()).set_scale(0.1f);

    const auto rotation_axis = glm::normalize(glm::vec3{ 1.0f, 0.3f, 0.5f });

    for (const auto [i, position] : container_positions | std::views::enumerate)
    {
        auto& container = _containers.emplace_back(position);
        auto angle = 0.35f * static_cast<float>(i);
        container.rotate(angle, rotation_axis);
    }
}

auto ContainersScene::on_load() -> void
{
    zth::Renderer::set_camera(_camera);
    zth::Renderer::set_directional_light(_directional_light);
    zth::Renderer::set_point_light(_point_light);
}

auto ContainersScene::on_update() -> void
{
    update_ui();

    if (!zth::Window::cursor_enabled())
        _camera_controller.on_update();

    _light_marker.set_translation(_point_light->translation());
    _light_marker_material.albedo = _point_light->properties.color;

    zth::Renderer::draw(_light_marker, _light_marker_material);

    for (const auto& container : _containers)
        zth::Renderer::draw(container, _container_material);
}

auto ContainersScene::on_event(const zth::Event& event) -> void
{
    switch (event.type())
    {
        using enum zth::EventType;
    case WindowResized:
    {
        auto window_resized_event = event.window_resized_event();
        on_window_resized_event(window_resized_event);
    }
    break;
    default:
        break;
    }
}

auto ContainersScene::on_window_resized_event(const zth::WindowResizedEvent& event) const -> void
{
    auto new_size = event.new_size;
    _camera->set_aspect_ratio(static_cast<float>(new_size.x) / static_cast<float>(new_size.y));
}

auto ContainersScene::update_ui() -> void
{
    _directional_light_ui.on_update();
    _point_light_ui.on_update();
}