#include "character.hpp"

#include "scripts/ball_spawner.hpp"

Character::Character() : Scene("Character") {}

auto Character::on_load() -> void
{
    // --- Camera ---
    _player.transform().translate(zth::math::world_up * 5.0f).scale(2.0f);
    _player.emplace_or_replace<zth::CameraComponent>();
    _player.emplace_or_replace<zth::ScriptComponent>(zth::make_unique<zth::scripts::CharacterController>());

    // --- Crosshair ---
    _crosshair.emplace_or_replace<zth::SpriteRenderer2DComponent>(
        zth::textures::white(), zth::Rect<zth::u32>{
                                    .position = glm::ivec2{ zth::Window::size() } / 2 + glm::ivec2{ -2, 2 },
                                    .size = glm::uvec2{ 4u, 4u },
                                });

    // --- Floor ---
    _floor.transform().scale(glm::vec3{ 100.0f, 1.0f, 100.0f });
    _floor.emplace_or_replace<zth::BoxColliderComponent>();
    _floor.emplace_or_replace<zth::MeshRendererComponent>(zth::meshes::cube());
    _floor.emplace_or_replace<zth::MaterialComponent>(zth::materials::plain());

    // --- Ball Spawner ---
    _ball_spawner.emplace_or_replace<zth::ScriptComponent>(zth::make_unique<scripts::BallSpawner>());

    // --- Directional Light ---
    _directional_light.transform().set_direction(glm::normalize(glm::vec3{ 0.0f, -1.0f, -1.0f }));
    _directional_light.emplace_or_replace<zth::LightComponent>(zth::LightType::Directional);
}
