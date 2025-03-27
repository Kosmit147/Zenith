#include "scene.hpp"

#include <battery/embed.hpp>

#include "cube_script.hpp"

namespace {

const auto wall_texture = b::embed<"assets/wall.jpg">().data();

const auto camera_transform_component =
    zth::TransformComponent{ glm::vec3{ 0.0f, 0.0f, 5.0f }, zth::math::world_forward };

const auto camera_camera_component = zth::CameraComponent{
    .aspect_ratio = 16.0f / 9.0f,
    .fov = glm::radians(45.0f),
};

} // namespace

Scene::Scene() : _cube_texture(wall_texture), _cube_material{ .diffuse_map = &_cube_texture }
{
    _camera.emplace_or_replace<zth::TransformComponent>(camera_transform_component);
    _camera.emplace_or_replace<zth::CameraComponent>(camera_camera_component);
    _camera.emplace_or_replace<zth::ScriptComponent>(std::make_unique<zth::scripts::FlyCamera>());

    _cube.emplace_or_replace<zth::MeshComponent>(&zth::meshes::cube_mesh());
    _cube.emplace_or_replace<zth::MaterialComponent>(&_cube_material);

    _light.emplace_or_replace<zth::TransformComponent>(glm::vec3{ 0.0f },
                                                       glm::normalize(glm::vec3{ 0.0f, -1.0f, -1.0f }));
    _light.emplace_or_replace<zth::LightComponent>(zth::DirectionalLight{});

    _cube.emplace_or_replace<zth::ScriptComponent>(std::make_unique<CubeScript>());
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
    auto new_size = event.new_size;

    auto& camera = _camera.get<zth::CameraComponent>();
    camera.aspect_ratio = static_cast<float>(new_size.x) / static_cast<float>(new_size.y);
}
