#include "scene.hpp"

#include <battery/embed.hpp>

namespace {

const auto cobble_texture_data = b::embed<"assets/cobble.png">().data();

const zth::TextureParams cobble_texture_params = {
    .min_filter = zth::TextureMinFilter::nearest_mipmap_linear,
    .mag_filter = zth::TextureMagFilter::nearest,
};

const auto light_direction = glm::normalize(glm::vec3{ 1.0f, -1.0f, 0.2f });

} // namespace

Scene::Scene()
    : _block_texture(cobble_texture_data, cobble_texture_params),
      _block_material{ .diffuse_map = &_block_texture, .specular = glm::vec3{ 0.1f } },
      _light(std::make_shared<zth::DirectionalLight>(light_direction))
{
    {
        // platform

        constexpr auto xs = std::views::iota(-20, 20);
        constexpr auto ys = std::views::iota(-20, 0);
        constexpr auto zs = std::views::iota(-20, 20);
        constexpr auto coords = std::views::cartesian_product(xs, ys, zs);

        for (const auto [x, y, z] : coords)
            _blocks.emplace_back(glm::ivec3{ x, y, z });
    }

    {
        // wall

        constexpr auto xs = std::views::iota(-5, 6);
        constexpr auto ys = std::views::iota(0, 5);
        constexpr auto zs = std::views::iota(0, 1);
        constexpr auto coords = std::views::cartesian_product(xs, ys, zs);

        for (const auto [x, y, z] : coords)
            _blocks.emplace_back(glm::ivec3{ x, y, z });
    }
}

auto Scene::on_load() -> void
{
    auto light_blue = glm::vec4{ 0.643f, 0.816f, 0.91f, 1.0f };
    zth::Renderer::set_clear_color(light_blue);
    zth::Renderer::set_camera(_player.camera());
    zth::Renderer::set_directional_light(_light);
}

auto Scene::on_update() -> void
{
    _player.on_update();

    for (const auto& block : _blocks)
        zth::Renderer::draw(block, _block_material);
}

auto Scene::on_event(const zth::Event& event) -> void
{
    _player.on_event(event);
}
