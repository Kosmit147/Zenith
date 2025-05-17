#include "containers.hpp"

#include "embedded.hpp"
#include "scripts/camera.hpp"
#include "scripts/light.hpp"

namespace {

const auto directional_light_light_component = zth::LightComponent{
    zth::DirectionalLight{
        .properties = {
            .ambient = glm::vec3{ 0.1f },
            .diffuse = glm::vec3{ 0.25f },
            .specular = glm::vec3{ 0.5f },
        },
    },
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

using namespace zth::hashed_string_literals;

constexpr auto container_diffuse_map_asset_id = "container_diffuse_map"_hs;
constexpr auto container_specular_map_asset_id = "container_specular_map"_hs;
constexpr auto container_material_asset_id = "container_material"_hs;
constexpr auto point_light_material_asset_id = "point_light_material"_hs;

} // namespace

Containers::Containers() : Scene("Containers")
{
    // clang-format off

    auto container_diffuse = zth::AssetManager::emplace<zth::gl::Texture2D>(
        container_diffuse_map_asset_id,
        zth::gl::Texture2D::from_file_data(embedded::container2_diffuse_map_data))->get();

    auto container_specular = zth::AssetManager::emplace<zth::gl::Texture2D>(
        container_specular_map_asset_id,
        zth::gl::Texture2D::from_file_data(embedded::container2_specular_map_data))->get();

    auto container_material = zth::AssetManager::emplace<zth::Material>(
        container_material_asset_id,
        zth::Material{ .diffuse_map = container_diffuse,
                       .specular_map = container_specular })->get();

    auto point_light_material = zth::AssetManager::emplace<zth::Material>(
        point_light_material_asset_id,
        zth::Material{ .shader = zth::shaders::flat_color() })->get();

    // clang-format on

    // --- Camera ---
    _camera.transform().translate(glm::vec3{ 0.0f, 0.0f, 5.0f });
    _camera.emplace_or_replace<zth::ScriptComponent>(zth::make_unique<scripts::Camera>());

    // --- Directional Light ---
    _directional_light.transform().set_direction(glm::normalize(glm::vec3{ 0.0f, -1.0f, -1.0f }));
    _directional_light.emplace_or_replace<zth::LightComponent>(directional_light_light_component);

    // --- Point Light ---
    _point_light.transform().translate(glm::vec3{ -0.7f, 1.3f, 1.7f }).scale(0.1f);
    _point_light.emplace_or_replace<zth::LightComponent>(point_light_light_component);
    _point_light.emplace_or_replace<zth::MeshRendererComponent>(zth::meshes::sphere());
    _point_light.emplace_or_replace<zth::MaterialComponent>(point_light_material);
    _point_light.emplace_or_replace<zth::ScriptComponent>(zth::make_unique<scripts::Light>(point_light_material));

    // --- Ambient Light ---
    _ambient_light.emplace_or_replace<zth::LightComponent>(ambient_light_light_component);

    // --- Containers ---
    for (const auto [i, position] : container_positions | std::views::enumerate)
    {
        auto container = create_entity(zth::format("Container {}", i));

        container.emplace_or_replace<zth::MeshRendererComponent>(zth::meshes::cube());
        container.emplace_or_replace<zth::MaterialComponent>(container_material);

        const auto rotation_axis = glm::normalize(glm::vec3{ 1.0f, 0.3f, 0.5f });
        auto angle = 0.35f * static_cast<float>(i);

        auto& container_transform = container.get<zth::TransformComponent>();
        container_transform.translate(position);
        container_transform.rotate(angle, rotation_axis);
    }
}

auto Containers::on_unload() -> void
{
    auto success = zth::AssetManager::remove<zth::gl::Texture2D>(container_diffuse_map_asset_id);
    ZTH_ASSERT(success);
    success = zth::AssetManager::remove<zth::gl::Texture2D>(container_specular_map_asset_id);
    ZTH_ASSERT(success);
    success = zth::AssetManager::remove<zth::Material>(container_material_asset_id);
    ZTH_ASSERT(success);
    success = zth::AssetManager::remove<zth::Material>(point_light_material_asset_id);
    ZTH_ASSERT(success);
}
