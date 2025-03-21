#include "main_scene.hpp"

#include "embedded.hpp"

namespace {

constexpr auto camera_position = glm::vec3{ 0.0f, 0.0f, 5.0f };
constexpr auto camera_forward = glm::vec3{ 0.0f, 0.0f, -1.0f };

constexpr auto aspect_ratio = 16.0f / 9.0f;
constexpr auto fov = glm::radians(45.0f);

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
      _point_light_3_material{ .shader = &zth::shaders::flat_color() },
      _camera(std::make_shared<zth::PerspectiveCamera>(camera_position, camera_forward, aspect_ratio, fov)),
      _camera_controller(_camera)
{
    auto& registry = Scene::registry();

    // --- Cube ---

    registry.emplace_or_replace<zth::MeshComponent>(_cube, &zth::meshes::cube_mesh());
    registry.emplace_or_replace<zth::MaterialComponent>(_cube, &_cube_material);

    // --- Lights ---

    registry.emplace_or_replace<zth::TransformComponent>(_directional_light, directional_light_transform_component);
    registry.emplace_or_replace<zth::LightComponent>(_directional_light, directional_light_light_component);

    registry.emplace_or_replace<zth::TransformComponent>(_point_light_1, point_light_1_transform_component);
    registry.emplace_or_replace<zth::LightComponent>(_point_light_1, point_light_1_light_component);
    registry.emplace_or_replace<zth::MeshComponent>(_point_light_1, &zth::meshes::sphere_mesh());
    registry.emplace_or_replace<zth::MaterialComponent>(_point_light_1, &_point_light_1_material);

    registry.emplace_or_replace<zth::TransformComponent>(_point_light_2, point_light_2_transform_component);
    registry.emplace_or_replace<zth::LightComponent>(_point_light_2, point_light_2_light_component);
    registry.emplace_or_replace<zth::MeshComponent>(_point_light_2, &zth::meshes::sphere_mesh());
    registry.emplace_or_replace<zth::MaterialComponent>(_point_light_2, &_point_light_2_material);

    registry.emplace_or_replace<zth::TransformComponent>(_point_light_3, point_light_3_transform_component);
    registry.emplace_or_replace<zth::LightComponent>(_point_light_3, point_light_3_light_component);
    registry.emplace_or_replace<zth::MeshComponent>(_point_light_3, &zth::meshes::sphere_mesh());
    registry.emplace_or_replace<zth::MaterialComponent>(_point_light_3, &_point_light_3_material);

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

auto MainScene::on_load() -> void
{
    zth::Renderer::set_camera(_camera);
}

auto MainScene::on_update() -> void
{
    update_ui();

    if (!zth::Window::cursor_enabled())
        _camera_controller.on_update();

    auto& registry = Scene::registry();

    auto& light_component_1 = registry.get<const zth::LightComponent>(_point_light_1);
    auto& light_component_2 = registry.get<const zth::LightComponent>(_point_light_2);
    auto& light_component_3 = registry.get<const zth::LightComponent>(_point_light_3);

    _point_light_1_material.albedo = light_component_1.point_light().properties.color;
    _point_light_2_material.albedo = light_component_2.point_light().properties.color;
    _point_light_3_material.albedo = light_component_3.point_light().properties.color;
}

auto MainScene::on_event(const zth::Event& event) -> void
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

auto MainScene::on_window_resized_event(const zth::WindowResizedEvent& event) -> void
{
    auto new_width = event.new_size.x;
    auto new_height = event.new_size.y;
    _camera->set_aspect_ratio(static_cast<float>(new_width) / static_cast<float>(new_height));
}

auto MainScene::update_ui() -> void
{
    _material_panel.draw();
}
