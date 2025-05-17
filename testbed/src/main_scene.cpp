#include "main_scene.hpp"

#include "scripts/camera.hpp"
#include "scripts/light.hpp"

namespace {

const auto directional_light_light_component = zth::LightComponent{ zth::DirectionalLight{
    .properties = { .color = zth::colors::black },
} };

} // namespace

MainScene::MainScene() : Scene("Main Scene")
{
    // --- Camera ---
    _camera.transform().translate(glm::vec3{ 0.0f, 0.0f, 5.0f });
    _camera.emplace_or_replace<zth::ScriptComponent>(zth::make_unique<scripts::Camera>());

    // --- Cube ---
    _cube.emplace_or_replace<zth::MeshRendererComponent>(zth::meshes::cube());
    _cube.emplace_or_replace<zth::MaterialComponent>(_cube_material);

    // --- Lights ---
    _directional_light.transform().set_direction(glm::normalize(glm::vec3{ 0.0f, -1.0f, 0.0f }));
    _directional_light.emplace_or_replace<zth::LightComponent>(directional_light_light_component);

    _point_light_1.transform().translate(glm::vec3{ -0.7f, 1.3f, 1.7f }).scale(0.1f);
    _point_light_1.emplace_or_replace<zth::LightComponent>(zth::LightType::Point);
    _point_light_1.emplace_or_replace<zth::MeshRendererComponent>(zth::meshes::sphere());
    _point_light_1.emplace_or_replace<zth::MaterialComponent>(_point_light_1_material);
    _point_light_1.emplace_or_replace<zth::ScriptComponent>(zth::make_unique<scripts::Light>(_point_light_1_material));

    _point_light_2.transform().translate(glm::vec3{ -0.2f, 2.3f, 0.1f }).scale(0.1f);
    _point_light_2.emplace_or_replace<zth::LightComponent>(zth::LightType::Point);
    _point_light_2.emplace_or_replace<zth::MeshRendererComponent>(zth::meshes::sphere());
    _point_light_2.emplace_or_replace<zth::MaterialComponent>(_point_light_2_material);
    _point_light_2.emplace_or_replace<zth::ScriptComponent>(zth::make_unique<scripts::Light>(_point_light_2_material));

    _point_light_3.transform().translate(glm::vec3{ -1.7f, 0.0f, 2.0f }).scale(0.1f);
    _point_light_3.emplace_or_replace<zth::LightComponent>(zth::LightType::Point);
    _point_light_3.emplace_or_replace<zth::MeshRendererComponent>(zth::meshes::sphere());
    _point_light_3.emplace_or_replace<zth::MaterialComponent>(_point_light_3_material);
    _point_light_3.emplace_or_replace<zth::ScriptComponent>(zth::make_unique<scripts::Light>(_point_light_3_material));
}
