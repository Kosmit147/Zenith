#include "containers.hpp"

#include "embedded.hpp"

namespace {

const auto camera_transform_component =
    zth::TransformComponent{ glm::vec3{ 0.0f, 0.0f, 5.0f }, zth::math::world_forward };

const auto camera_camera_component = zth::CameraComponent{
    .aspect_ratio = 16.0f / 9.0f,
    .fov = glm::radians(45.0f),
};

const auto camera_light_component = zth::LightComponent{
    zth::SpotLight {
        .attenuation = {
            .linear = 0.09f,
            .quadratic = 0.032f,
        },
    },
};

const auto directional_light_transform_component = zth::TransformComponent{
    glm::vec3{ 0.0f },
    glm::normalize(glm::vec3{ 0.0f, -1.0f, -1.0f }),
};

const auto directional_light_light_component = zth::LightComponent{
    zth::DirectionalLight{
        .properties = {
            .ambient = glm::vec3{ 0.1f },
            .diffuse = glm::vec3{ 0.25f },
            .specular = glm::vec3{ 0.5f },
        },
    },
};

const auto point_light_transform_component = zth::TransformComponent{
    glm::vec3{ -0.7f, 1.3f, 1.7f },
    glm::identity<glm::quat>(),
    0.1f,
};

const auto point_light_light_component = zth::LightComponent {
    zth::PointLight {
        .properties = {
            .color = zth::colors::magenta,
        },
        .attenuation = {
            .linear = 0.09f,
            .quadratic = 0.032f,
        },
    },
};

const auto ambient_light_light_component = zth::LightComponent{ zth::AmbientLight{
    .ambient = glm::vec3{ 0.05f, 0.05f, 0.05f },
} };

constexpr std::array container_positions = { glm::vec3{ 0.0f, 0.0f, 0.0f },    glm::vec3{ 2.0f, 5.0f, -15.0f },
                                             glm::vec3{ -1.5f, -2.2f, -2.5f }, glm::vec3{ -3.8f, -2.0f, -12.3f },
                                             glm::vec3{ 2.4f, -0.4f, -3.5f },  glm::vec3{ -1.7f, 3.0f, -7.5f },
                                             glm::vec3{ 1.3f, -2.0f, -2.5f },  glm::vec3{ 1.5f, 2.0f, -2.5f },
                                             glm::vec3{ 1.5f, 0.2f, -1.5f },   glm::vec3{ -1.3f, 1.0f, -1.5f } };

} // namespace

Containers::Containers()
    : _diffuse_map{ embedded::container2_diffuse_map_data }, _specular_map{ embedded::container2_specular_map_data },
      _container_material{ .diffuse_map = &_diffuse_map, .specular_map = &_specular_map },
      _point_light_material{ .shader = &zth::shaders::flat_color() }
{
    // --- Camera ---

    _camera.emplace_or_replace<zth::TransformComponent>(camera_transform_component);
    _camera.emplace_or_replace<zth::CameraComponent>(camera_camera_component);
    _camera.emplace_or_replace<zth::ScriptComponent>(std::make_unique<zth::scripts::FlyCamera>());
    _camera.emplace_or_replace<zth::LightComponent>(camera_light_component);

    // --- Directional Light ---

    _directional_light.emplace_or_replace<zth::TransformComponent>(directional_light_transform_component);
    _directional_light.emplace_or_replace<zth::LightComponent>(directional_light_light_component);

    // --- Point Light ---
    _point_light.emplace_or_replace<zth::TransformComponent>(point_light_transform_component);
    _point_light.emplace_or_replace<zth::LightComponent>(point_light_light_component);
    _point_light.emplace_or_replace<zth::MeshComponent>(&zth::meshes::sphere_mesh());
    _point_light.emplace_or_replace<zth::MaterialComponent>(&_point_light_material);

    // --- Ambient Light ---
    _ambient_light.emplace_or_replace<zth::LightComponent>(ambient_light_light_component);

    // --- Containers ---

    for (const auto [i, position] : container_positions | std::views::enumerate)
    {
        auto label = zth::format("Container {}", i);
        auto& container = _containers.emplace_back(create_entity(label));

        container.emplace_or_replace<zth::MeshComponent>(&zth::meshes::cube_mesh());
        container.emplace_or_replace<zth::MaterialComponent>(&_container_material);

        const auto rotation_axis = glm::normalize(glm::vec3{ 1.0f, 0.3f, 0.5f });
        auto angle = 0.35f * static_cast<float>(i);

        auto& container_transform = container.get<zth::TransformComponent>();
        container_transform.translate(position);
        container_transform.rotate(angle, rotation_axis);
    }
}

auto Containers::on_event(const zth::Event& event) -> void
{
    switch (event.type())
    {
        using enum zth::EventType;
    case KeyPressed:
    {
        auto key_pressed_event = event.key_pressed_event();
        on_key_pressed_event(key_pressed_event);
    }
    break;
    default:
        break;
    }
}

auto Containers::on_update() -> void
{
    auto& light = _point_light.get<const zth::LightComponent>();
    _point_light_material.albedo = light.point_light().properties.color;
}

auto Containers::on_key_pressed_event(const zth::KeyPressedEvent& event) -> void
{
    if (event.key == toggle_spotlight_key)
    {
        _spot_light_on = !_spot_light_on;

        auto& light = _camera.get<zth::LightComponent>();

        if (_spot_light_on)
            light.spot_light().properties.color = zth::colors::white;
        else
            light.spot_light().properties.color = zth::colors::black;
    }
}
