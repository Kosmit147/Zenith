#include "main_scene.hpp"

#include <battery/embed.hpp>

#include "cube_script.hpp"

namespace {

const auto wall_texture = b::embed<"assets/wall.jpg">().data();

} // namespace

MainScene::MainScene()
    : Scene("Main Scene"),
      _cube_texture{ std::make_shared<zth::gl::Texture2D>(zth::gl::Texture2D::from_file_data(wall_texture)) },
      _cube_material{ std::make_shared<zth::Material>(zth::Material{ .diffuse_map = _cube_texture }) }
{
    _camera.transform().translate(glm::vec3{ 0.0f, 0.0f, 5.0f });
    _camera.emplace_or_replace<zth::CameraComponent>();
    _camera.emplace_or_replace<zth::ScriptComponent>(zth::make_unique<zth::scripts::FlyCamera>());

    _cube.emplace_or_replace<zth::MeshRendererComponent>(zth::meshes::cube());
    _cube.emplace_or_replace<zth::MaterialComponent>(_cube_material);

    _light.transform().set_direction(glm::normalize(glm::vec3{ 0.0f, -1.0f, -1.0f }));
    _light.emplace_or_replace<zth::LightComponent>(zth::DirectionalLight{});

    _cube.emplace_or_replace<zth::ScriptComponent>(zth::make_unique<scripts::Cube>());
}
