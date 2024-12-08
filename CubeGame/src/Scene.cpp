#include "Scene.hpp"

#include <battery/embed.hpp>

#include <ranges>

namespace {

const auto cobble_texture_data = b::embed<"assets/cobble.png">().vec();

const zth::TextureParams cobble_texture_params = {
    .min_filter = zth::TextureMinFilter::nearest_mipmap_linear,
    .mag_filter = zth::TextureMagFilter::nearest,
};

} // namespace

Scene::Scene()
    : _block_texture(cobble_texture_data, cobble_texture_params),
      _block_material(zth::shaders::texture_shader, &_block_texture)
{
    constexpr auto xs = std::views::iota(-20, 20);
    constexpr auto ys = std::views::iota(-20, 0);
    constexpr auto zs = std::views::iota(-20, 20);
    constexpr auto coords = std::views::cartesian_product(xs, ys, zs);

    for (const auto [x, y, z] : coords)
        _blocks.emplace_back(glm::ivec3{ x, y, z });
}

auto Scene::on_load() -> void
{
    auto light_blue = glm::vec4(0.643f, 0.816f, 0.91f, 1.0f);
    zth::Renderer::set_clear_color(light_blue);
    zth::Renderer::set_camera(_player.camera());
}

auto Scene::on_update() -> void
{
    _player.on_update();

    for (const auto& block : _blocks)
        zth::Renderer::submit(block, _block_material);
}

auto Scene::on_event(const zth::Event& event) -> void
{
    _player.on_event(event);
}
