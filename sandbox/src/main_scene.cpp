#include "main_scene.hpp"

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

MainScene::MainScene()
    : Scene("Main Scene"),
      _cube_texture{ std::make_shared<zth::gl::Texture2D>(zth::gl::Texture2D::from_file_data(wall_texture)) },
      _cube_material{ std::make_shared<zth::Material>(zth::Material{ .diffuse_map = _cube_texture }) }
{
    _camera.emplace_or_replace<zth::TransformComponent>(camera_transform_component);
    _camera.emplace_or_replace<zth::CameraComponent>(camera_camera_component);
    _camera.emplace_or_replace<zth::ScriptComponent>(zth::make_unique<zth::scripts::FlyCamera>());

    _cube.emplace_or_replace<zth::MeshComponent>(zth::meshes::cube());
    _cube.emplace_or_replace<zth::MaterialComponent>(_cube_material);

    _light.emplace_or_replace<zth::TransformComponent>(glm::vec3{ 0.0f },
                                                       glm::normalize(glm::vec3{ 0.0f, -1.0f, -1.0f }));
    _light.emplace_or_replace<zth::LightComponent>(zth::DirectionalLight{});

    _cube.emplace_or_replace<zth::ScriptComponent>(zth::make_unique<CubeScript>());
}
