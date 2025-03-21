#include "scene.hpp"

#include <battery/embed.hpp>

namespace {

const auto cobble_texture_data = b::embed<"assets/cobble.png">().data();

const zth::gl::TextureParams cobble_texture_params = {
    .min_filter = zth::gl::TextureMinFilter::nearest_mipmap_linear,
    .mag_filter = zth::gl::TextureMagFilter::nearest,
};

const auto directional_light_direction = glm::normalize(glm::vec3{ 1.0f, -1.0f, 0.2f });

} // namespace

Scene::Scene()
    : _block_texture(cobble_texture_data, cobble_texture_params),
      _block_material{ .diffuse_map = &_block_texture, .specular = glm::vec3{ 0.1f } }
{
    auto& registry = Scene::registry();

    auto& light_transform = registry.get<zth::TransformComponent>(_directional_light);
    light_transform.set_direction(directional_light_direction);
    registry.emplace_or_replace<zth::LightComponent>(_directional_light, zth::DirectionalLight{});

    std::size_t counter = 0;

    {
        // Platform.

        constexpr auto xs = std::views::iota(-20, 20);
        constexpr auto ys = std::views::iota(-20, 0);
        constexpr auto zs = std::views::iota(-20, 20);
        constexpr auto coords = std::views::cartesian_product(xs, ys, zs);

        for (const auto [x, y, z] : coords)
        {
            auto name = ZTH_FORMAT("Block {}", counter++);
            auto& block = _blocks.emplace_back(create_entity(name));

            auto& block_transform = registry.get<zth::TransformComponent>(block);
            block_transform.set_translation(glm::ivec3{ x, y, z });

            registry.emplace<zth::MeshComponent>(block, &zth::meshes::cube_mesh());
            registry.emplace<zth::MaterialComponent>(block, &_block_material);
        }
    }

    {
        // Wall.

        constexpr auto xs = std::views::iota(-5, 6);
        constexpr auto ys = std::views::iota(0, 5);
        constexpr auto zs = std::views::iota(0, 1);
        constexpr auto coords = std::views::cartesian_product(xs, ys, zs);

        for (const auto [x, y, z] : coords)
        {
            auto name = ZTH_FORMAT("Block {}", counter++);
            auto& block = _blocks.emplace_back(create_entity(name));

            auto& block_transform = registry.get<zth::TransformComponent>(block);
            block_transform.set_translation(glm::ivec3{ x, y, z });

            registry.emplace<zth::MeshComponent>(block, &zth::meshes::cube_mesh());
            registry.emplace<zth::MaterialComponent>(block, &_block_material);
        }
    }
}

auto Scene::on_load() -> void
{
    auto light_blue = glm::vec4{ 0.643f, 0.816f, 0.91f, 1.0f };
    zth::Renderer::set_clear_color(light_blue);
    zth::Renderer::set_camera(_player.camera());
}

auto Scene::on_update() -> void
{
    _player.on_update();
}

auto Scene::on_event(const zth::Event& event) -> void
{
    _player.on_event(event);
}
