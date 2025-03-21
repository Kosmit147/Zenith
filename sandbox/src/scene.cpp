#include "scene.hpp"

#include <battery/embed.hpp>

namespace {

const auto wall_texture = b::embed<"assets/wall.jpg">().data();

constexpr auto camera_position = glm::vec3{ 0.0f, 0.0f, 5.0f };
constexpr auto camera_forward = glm::vec3{ 0.0f, 0.0f, -1.0f };

constexpr auto aspect_ratio = 16.0f / 9.0f;
constexpr auto fov = glm::radians(45.0f);

} // namespace

Scene::Scene()
    : _cube_texture(wall_texture), _cube_material{ .diffuse_map = &_cube_texture },
      _camera(std::make_shared<zth::PerspectiveCamera>(camera_position, camera_forward, aspect_ratio, fov)),
      _camera_controller(_camera)
{
    auto& registry = Scene::registry();

    registry.emplace_or_replace<zth::MeshComponent>(_cube, &zth::meshes::cube_mesh());
    registry.emplace_or_replace<zth::MaterialComponent>(_cube, &_cube_material);

    registry.emplace_or_replace<zth::TransformComponent>(_light, glm::vec3{ 0.0f },
                                                         glm::normalize(glm::vec3{ 0.0f, -1.0f, -1.0f }));
    registry.emplace_or_replace<zth::LightComponent>(_light, zth::DirectionalLight{});
}

auto Scene::on_load() -> void
{
    zth::Renderer::set_camera(_camera);
}

auto Scene::on_update() -> void
{
    const auto time = zth::Time::time<float>();

    if (!zth::Window::cursor_enabled())
        _camera_controller.on_update();

    auto& registry = Scene::registry();

    auto& cube_transform = registry.get<zth::TransformComponent>(_cube);

    cube_transform.rotate(0.0005f * time, glm::normalize(glm::vec3{ 0.0f, 1.0f, 0.0f }));
    cube_transform.rotate(0.0005f * time, glm::normalize(glm::vec3{ -0.3f, 0.1f, 0.7f }));
}

auto Scene::on_event(const zth::Event& event) -> void
{
    if (event.type() == zth::EventType::WindowResized)
    {
        auto window_resized_event = event.window_resized_event();
        on_window_resized_event(window_resized_event);
    }
}

auto Scene::on_window_resized_event(const zth::WindowResizedEvent& event) const -> void
{
    auto new_width = event.new_size.x;
    auto new_height = event.new_size.y;
    _camera->set_aspect_ratio(static_cast<float>(new_width) / static_cast<float>(new_height));
}
