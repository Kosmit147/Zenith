#include "Scene.hpp"

#include <battery/embed.hpp>

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
{}

auto Scene::on_load() -> void
{
    auto light_blue = glm::vec4(0.643f, 0.816f, 0.91f, 1.0f);
    zth::Renderer::set_clear_color(light_blue);
    zth::Renderer::set_camera(_player.camera());
}

auto Scene::on_update() -> void
{
    _player.on_update();

    auto rotation_speed = 0.0005f;

    _block.rotate(rotation_speed, glm::normalize(glm::vec3{ 0.0f, 1.0f, 0.0f }));
    _block.rotate(rotation_speed, glm::normalize(glm::vec3{ -0.3f, 0.1f, 0.7f }));
    _block.rotate(rotation_speed, glm::normalize(glm::vec3{ 0.9f, 0.4f, 0.4f }));

    zth::Renderer::submit(_block, _block_material);
}

auto Scene::on_event(const zth::Event& event) -> void
{
    _player.on_event(event);
}
