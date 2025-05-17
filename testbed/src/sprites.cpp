#include "sprites.hpp"

#include "embedded.hpp"

namespace {

using namespace zth::hashed_string_literals;

constexpr auto container_texture_asset_id = "container_texture"_hs;
constexpr auto cobble_texture_asset_id = "cobble_texture"_hs;
constexpr auto emoji_texture_asset_id = "emoji_texture"_hs;

} // namespace

Sprites::Sprites() : Scene("Sprites") {}

auto Sprites::on_load() -> void
{
    // clang-format off

    auto container_texture = zth::AssetManager::emplace<zth::gl::Texture2D>(
        container_texture_asset_id,
        zth::gl::Texture2D::from_file_data(embedded::container_diffuse_map_data))->get();

    auto cobble_texture = zth::AssetManager::emplace<zth::gl::Texture2D>(
        cobble_texture_asset_id,
        zth::gl::Texture2D::from_file_data(embedded::cobble_diffuse_map_data, zth::gl::TextureParams{
            .mag_filter = zth::gl::TextureMagFilter::nearest,
        }))->get();

    auto emoji_texture = zth::AssetManager::emplace<zth::gl::Texture2D>(
        emoji_texture_asset_id,
        zth::gl::Texture2D::from_file_data(embedded::emoji_diffuse_map_data))->get();

    // clang-format on

    // --- Camera ---
    _camera.transform().translate(glm::vec3{ 0.0f, 0.0f, 5.0f });
    _camera.emplace_or_replace<zth::CameraComponent>();
    _camera.emplace_or_replace<zth::ScriptComponent>(zth::make_unique<zth::scripts::FlyCamera>());

    for (std::size_t i = 0; i < 3; i++)
    {
        auto cobble = create_entity(zth::format("Cobble {}", i));
        cobble.emplace_or_replace<zth::SpriteRenderer2DComponent>(zth::SpriteRenderer2DComponent{
            .texture = cobble_texture,
            .rect = {
                .position = glm::uvec2{ 600 + i * 200, 600 },
                .size = glm::uvec2{ 200, 200 },
            },
            .color = zth::Random::rgba_color(),
        });
    }

    auto container = create_entity("Container");
    container.emplace_or_replace<zth::SpriteRenderer2DComponent>(zth::SpriteRenderer2DComponent{
        .texture = container_texture,
        .rect = {
            .position = glm::uvec2{ 400, 400 },
            .size = glm::uvec2{ 200, 200 },
        },
        .color = zth::Random::rgba_color(),
    });

    auto emoji = create_entity("Emoji");
    emoji.emplace_or_replace<zth::SpriteRenderer2DComponent>(zth::SpriteRenderer2DComponent{
        .texture = emoji_texture,
        .rect = {
            .position = glm::uvec2{ 600, 400 },
            .size = glm::uvec2{ 200, 200 },
        },
        .color = zth::Random::rgba_color(),
    });
}

auto Sprites::on_unload() -> void
{
    auto success = zth::AssetManager::remove<zth::gl::Texture2D>(container_texture_asset_id);
    ZTH_ASSERT(success);
    success = zth::AssetManager::remove<zth::gl::Texture2D>(cobble_texture_asset_id);
    ZTH_ASSERT(success);
    success = zth::AssetManager::remove<zth::gl::Texture2D>(emoji_texture_asset_id);
    ZTH_ASSERT(success);
}
