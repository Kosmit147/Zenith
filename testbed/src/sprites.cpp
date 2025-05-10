#include "sprites.hpp"

#include "embedded.hpp"

namespace {

const auto camera_transform_component =
    zth::TransformComponent{ glm::vec3{ 0.0f, 0.0f, 5.0f }, zth::math::world_forward };

const auto camera_camera_component = zth::CameraComponent{
    .aspect_ratio = 16.0f / 9.0f,
    .fov = glm::radians(45.0f),
};

using namespace zth::hashed_string_literals;

constexpr auto container_texture_asset_id = "container_texture"_hs;
constexpr auto cobble_texture_asset_id = "cobble_texture"_hs;
constexpr auto emoji_texture_asset_id = "emoji_texture"_hs;

} // namespace

Sprites::Sprites() : Scene("Sprites")
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
    _camera.emplace_or_replace<zth::TransformComponent>(camera_transform_component);
    _camera.emplace_or_replace<zth::CameraComponent>(camera_camera_component);
    _camera.emplace_or_replace<zth::ScriptComponent>(zth::make_unique<zth::scripts::FlyCamera>());

    for (std::size_t i = 0; i < 3; i++)
    {
        auto cobble = create_entity(zth::format("Cobble {}", i));
        cobble.emplace_or_replace<zth::Sprite2DComponent>(zth::Sprite2DComponent{
            .texture = cobble_texture,
            .rect = {
                .position = glm::uvec2{ 600 + i * 200, 600 },
                .size = glm::uvec2{ 200, 200 },
            },
            .color = zth::colors::red,
        });
    }

    auto container = create_entity("Container");
    container.emplace_or_replace<zth::Sprite2DComponent>(zth::Sprite2DComponent{
        .texture = container_texture,
        .rect = {
            .position = glm::uvec2{ 400, 400 },
            .size = glm::uvec2{ 200, 200 },
        },
        .color = zth::colors::green,
    });

    auto emoji = create_entity("Emoji");
    emoji.emplace_or_replace<zth::Sprite2DComponent>(zth::Sprite2DComponent{
        .texture = emoji_texture,
        .rect = {
            .position = glm::uvec2{ 600, 400 },
            .size = glm::uvec2{ 200, 200 },
        },
        .color = zth::colors::blue,
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
