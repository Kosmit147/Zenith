#include "main_scene.hpp"

#include "zenith/script/camera_controller.hpp"

namespace {

const auto camera_transform_component =
    zth::TransformComponent{ glm::vec3{ 0.0f, 0.0f, 5.0f }, zth::math::world_forward };

const auto camera_camera_component = zth::CameraComponent{
    .aspect_ratio = 16.0f / 9.0f,
    .fov = glm::radians(45.0f),
};

const auto directional_light_transform_component = zth::TransformComponent{
    glm::vec3{ 0.0f },
    glm::normalize(glm::vec3{ 0.0f, -1.0f, 0.0f }),
};

const auto directional_light_light_component = zth::LightComponent{ zth::DirectionalLight{
    .properties = { .color = zth::colors::black },
} };

const auto point_light_1_transform_component = zth::TransformComponent{
    glm::vec3{ -0.7f, 1.3f, 1.7f },
    glm::identity<glm::quat>(),
    0.1f,
};

const auto point_light_1_light_component = zth::LightComponent{ zth::PointLight{} };

const auto point_light_2_transform_component = zth::TransformComponent{
    glm::vec3{ -0.2f, 2.3f, 0.1f },
    glm::identity<glm::quat>(),
    0.1f,
};

const auto point_light_2_light_component = zth::LightComponent{ zth::PointLight{} };

const auto point_light_3_transform_component = zth::TransformComponent{
    glm::vec3{ -1.7f, 0.0f, 2.0f },
    glm::identity<glm::quat>(),
    glm::vec3{ 0.1f },
};

const auto point_light_3_light_component = zth::LightComponent{ zth::PointLight{} };

} // namespace

MainScene::MainScene() : Scene("Main Scene")
{
    // --- Camera ---

    _camera.emplace_or_replace<zth::TransformComponent>(camera_transform_component);
    _camera.emplace_or_replace<zth::CameraComponent>(camera_camera_component);
    _camera.emplace_or_replace<zth::ScriptComponent>(zth::make_unique<zth::scripts::FlyCamera>());

    // --- Cube ---

    _cube.emplace_or_replace<zth::MeshComponent>(zth::meshes::cube());
    _cube.emplace_or_replace<zth::MaterialComponent>(_cube_material);

    // --- Lights ---

    _directional_light.emplace_or_replace<zth::TransformComponent>(directional_light_transform_component);
    _directional_light.emplace_or_replace<zth::LightComponent>(directional_light_light_component);

    _point_light_1.emplace_or_replace<zth::TransformComponent>(point_light_1_transform_component);
    _point_light_1.emplace_or_replace<zth::LightComponent>(point_light_1_light_component);
    _point_light_1.emplace_or_replace<zth::MeshComponent>(zth::meshes::sphere());
    _point_light_1.emplace_or_replace<zth::MaterialComponent>(_point_light_1_material);

    _point_light_2.emplace_or_replace<zth::TransformComponent>(point_light_2_transform_component);
    _point_light_2.emplace_or_replace<zth::LightComponent>(point_light_2_light_component);
    _point_light_2.emplace_or_replace<zth::MeshComponent>(zth::meshes::sphere());
    _point_light_2.emplace_or_replace<zth::MaterialComponent>(_point_light_2_material);

    _point_light_3.emplace_or_replace<zth::TransformComponent>(point_light_3_transform_component);
    _point_light_3.emplace_or_replace<zth::LightComponent>(point_light_3_light_component);
    _point_light_3.emplace_or_replace<zth::MeshComponent>(zth::meshes::sphere());
    _point_light_3.emplace_or_replace<zth::MaterialComponent>(_point_light_3_material);
}

auto MainScene::on_update() -> void
{
    auto& light_component_1 = _point_light_1.get<const zth::LightComponent>();
    auto& light_component_2 = _point_light_2.get<const zth::LightComponent>();
    auto& light_component_3 = _point_light_3.get<const zth::LightComponent>();

    _point_light_1_material->albedo = light_component_1.point_light().properties.color;
    _point_light_2_material->albedo = light_component_2.point_light().properties.color;
    _point_light_3_material->albedo = light_component_3.point_light().properties.color;
}
