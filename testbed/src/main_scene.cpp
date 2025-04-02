#include "main_scene.hpp"

#include "embedded.hpp"
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

const auto cobble_diffuse_map_params = zth::gl::TextureParams{
    .min_filter = zth::gl::TextureMinFilter::nearest_mipmap_linear,
    .mag_filter = zth::gl::TextureMagFilter::nearest,
};

} // namespace

MainScene::MainScene()
    : _point_light_1_material{ .shader = &zth::shaders::flat_color() },
      _point_light_2_material{ .shader = &zth::shaders::flat_color() },
      _point_light_3_material{ .shader = &zth::shaders::flat_color() }
{
    // --- Camera ---

    _camera.emplace_or_replace<zth::TransformComponent>(camera_transform_component);
    _camera.emplace_or_replace<zth::CameraComponent>(camera_camera_component);
    _camera.emplace_or_replace<zth::ScriptComponent>(std::make_unique<zth::scripts::FlyCamera>());

    // --- Cube ---

    _cube.emplace_or_replace<zth::MeshComponent>(&zth::meshes::cube_mesh());
    _cube.emplace_or_replace<zth::MaterialComponent>(&_cube_material);

    // --- Lights ---

    _directional_light.emplace_or_replace<zth::TransformComponent>(directional_light_transform_component);
    _directional_light.emplace_or_replace<zth::LightComponent>(directional_light_light_component);

    _point_light_1.emplace_or_replace<zth::TransformComponent>(point_light_1_transform_component);
    _point_light_1.emplace_or_replace<zth::LightComponent>(point_light_1_light_component);
    _point_light_1.emplace_or_replace<zth::MeshComponent>(&zth::meshes::sphere_mesh());
    _point_light_1.emplace_or_replace<zth::MaterialComponent>(&_point_light_1_material);

    _point_light_2.emplace_or_replace<zth::TransformComponent>(point_light_2_transform_component);
    _point_light_2.emplace_or_replace<zth::LightComponent>(point_light_2_light_component);
    _point_light_2.emplace_or_replace<zth::MeshComponent>(&zth::meshes::sphere_mesh());
    _point_light_2.emplace_or_replace<zth::MaterialComponent>(&_point_light_2_material);

    _point_light_3.emplace_or_replace<zth::TransformComponent>(point_light_3_transform_component);
    _point_light_3.emplace_or_replace<zth::LightComponent>(point_light_3_light_component);
    _point_light_3.emplace_or_replace<zth::MeshComponent>(&zth::meshes::sphere_mesh());
    _point_light_3.emplace_or_replace<zth::MaterialComponent>(&_point_light_3_material);

    _diffuse_maps.emplace_back(embedded::cobble_diffuse_map_data, cobble_diffuse_map_params);
    _diffuse_maps.emplace_back(embedded::container_diffuse_map_data);
    _diffuse_maps.emplace_back(embedded::container2_diffuse_map_data);
    _diffuse_maps.emplace_back(embedded::emoji_diffuse_map_data);
    _diffuse_maps.emplace_back(embedded::wall_diffuse_map_data);

    std::size_t i = 0;
    _material_panel.add_diffuse_map("Cobble", _diffuse_maps[i++]);
    _material_panel.add_diffuse_map("Container", _diffuse_maps[i++]);
    _material_panel.add_diffuse_map("Container2", _diffuse_maps[i++]);
    _material_panel.add_diffuse_map("Emoji", _diffuse_maps[i++]);
    _material_panel.add_diffuse_map("Wall", _diffuse_maps[i++]);

    _specular_maps.emplace_back(embedded::container2_specular_map_data);
    _material_panel.add_specular_map("Container2", _specular_maps[0]);

    _emission_maps.emplace_back(embedded::matrix_emission_map_data);
    _material_panel.add_emission_map("Matrix", _emission_maps[0]);
}

auto MainScene::on_update() -> void
{
    display_ui();

    auto& light_component_1 = _point_light_1.get<const zth::LightComponent>();
    auto& light_component_2 = _point_light_2.get<const zth::LightComponent>();
    auto& light_component_3 = _point_light_3.get<const zth::LightComponent>();

    _point_light_1_material.albedo = light_component_1.point_light().properties.color;
    _point_light_2_material.albedo = light_component_2.point_light().properties.color;
    _point_light_3_material.albedo = light_component_3.point_light().properties.color;
}

auto MainScene::display_ui() -> void
{
    _material_panel.display();
}
