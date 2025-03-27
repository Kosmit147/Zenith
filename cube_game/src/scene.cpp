#include "scene.hpp"

#include <battery/embed.hpp>

namespace {

const auto cobble_texture_data = b::embed<"assets/cobble.png">().data();

const auto camera_transform_component =
    zth::TransformComponent{ glm::vec3{ 0.0f, 0.0f, 5.0f }, zth::math::world_forward };

const auto camera_camera_component = zth::CameraComponent{
    .aspect_ratio = 16.0f / 9.0f,
    .fov = glm::radians(45.0f),
};

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
    _camera.emplace_or_replace<zth::TransformComponent>(camera_transform_component);
    _camera.emplace_or_replace<zth::CameraComponent>(camera_camera_component);
    _camera.emplace_or_replace<zth::ScriptComponent>(std::make_unique<zth::scripts::FlyCamera>());

    auto& light_transform = _directional_light.get<zth::TransformComponent>();
    light_transform.set_direction(directional_light_direction);
    _directional_light.emplace_or_replace<zth::LightComponent>(zth::DirectionalLight{});

    std::size_t counter = 0;

    {
        // Platform.

        constexpr auto xs = std::views::iota(-20, 20);
        constexpr auto ys = std::views::iota(-20, 0);
        constexpr auto zs = std::views::iota(-20, 20);
        constexpr auto coords = std::views::cartesian_product(xs, ys, zs);

        for (const auto [x, y, z] : coords)
        {
            auto name = zth::format("Block {}", counter++);
            auto& block = _blocks.emplace_back(create_entity(name));

            auto& block_transform = block.get<zth::TransformComponent>();
            block_transform.set_translation(glm::ivec3{ x, y, z });

            block.emplace<zth::MeshComponent>(&zth::meshes::cube_mesh());
            block.emplace<zth::MaterialComponent>(&_block_material);
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
            auto name = zth::format("Block {}", counter++);
            auto& block = _blocks.emplace_back(create_entity(name));

            auto& block_transform = block.get<zth::TransformComponent>();
            block_transform.set_translation(glm::ivec3{ x, y, z });

            block.emplace<zth::MeshComponent>(&zth::meshes::cube_mesh());
            block.emplace<zth::MaterialComponent>(&_block_material);
        }
    }
}

auto Scene::on_load() -> void
{
    auto light_blue = glm::vec4{ 0.643f, 0.816f, 0.91f, 1.0f };
    zth::Renderer::set_clear_color(light_blue);
}

auto Scene::on_event(const zth::Event& event) -> void
{
    if (event.type() == zth::EventType::WindowResized)
    {
        auto window_resized_event = event.window_resized_event();
        on_window_resized_event(window_resized_event);
    }
}

auto Scene::on_window_resized_event(const zth::WindowResizedEvent& event) const -> void
{
    auto new_size = event.new_size;

    auto& camera = _camera.get<zth::CameraComponent>();
    camera.aspect_ratio = static_cast<float>(new_size.x) / static_cast<float>(new_size.y);
}
