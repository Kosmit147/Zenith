#include "physics.hpp"

#include "scripts/ball_spawner.hpp"
#include "scripts/camera.hpp"

Physics::Physics() : Scene("Physics") {}

auto Physics::on_load() -> void
{
    static constexpr std::array test_quad_mesh = {
        // A ------------------
        zth::StandardVertex{ .position = glm::vec3{ 0.0f, 0.0f, 0.0f }, .normal = zth::math::world_up },
        zth::StandardVertex{ .position = glm::vec3{ 0.0f, 0.0f, -0.5f }, .normal = zth::math::world_up },
        zth::StandardVertex{ .position = glm::vec3{ 0.5f, 0.0f, -0.5f }, .normal = zth::math::world_up },
        zth::StandardVertex{ .position = glm::vec3{ 0.5f, 0.0f, 0.0f }, .normal = zth::math::world_up },
        // B ------------------
        zth::StandardVertex{ .position = glm::vec3{ 0.5f, 0.0f, 0.0f }, .normal = zth::math::world_up },
        zth::StandardVertex{ .position = glm::vec3{ 0.5f, 0.0f, -0.5f }, .normal = zth::math::world_up },
        zth::StandardVertex{ .position = glm::vec3{ 1.0f, 0.0f, -0.5f }, .normal = zth::math::world_up },
        zth::StandardVertex{ .position = glm::vec3{ 1.0f, 0.0f, 0.0f }, .normal = zth::math::world_up },
        // C ------------------
        zth::StandardVertex{ .position = glm::vec3{ 0.0f, 0.0f, -0.5f }, .normal = zth::math::world_up },
        zth::StandardVertex{ .position = glm::vec3{ 0.0f, 0.0f, -1.0f }, .normal = zth::math::world_up },
        zth::StandardVertex{ .position = glm::vec3{ 0.5f, 0.0f, -1.0f }, .normal = zth::math::world_up },
        zth::StandardVertex{ .position = glm::vec3{ 0.5f, 0.0f, -0.5f }, .normal = zth::math::world_up },
        // D ------------------
        zth::StandardVertex{ .position = glm::vec3{ 0.5f, 0.0f, -0.5f }, .normal = zth::math::world_up },
        zth::StandardVertex{ .position = glm::vec3{ 0.5f, 0.0f, -1.0f }, .normal = zth::math::world_up },
        zth::StandardVertex{ .position = glm::vec3{ 1.0f, 0.0f, -1.0f }, .normal = zth::math::world_up },
        zth::StandardVertex{ .position = glm::vec3{ 1.0f, 0.0f, -0.5f }, .normal = zth::math::world_up },
        // E ------------------
        zth::StandardVertex{ .position = glm::vec3{ 1.0f, 0.5f, -0.5f }, .normal = zth::math::world_forward },
        zth::StandardVertex{ .position = glm::vec3{ 1.0f, 0.0f, -0.5f }, .normal = zth::math::world_forward },
        zth::StandardVertex{ .position = glm::vec3{ 0.5f, 0.0f, -0.5f }, .normal = zth::math::world_forward },
        zth::StandardVertex{ .position = glm::vec3{ 0.5f, 0.5f, -0.5f }, .normal = zth::math::world_forward },
        // F ------------------
        zth::StandardVertex{ .position = glm::vec3{ 0.5f, 0.5f, -0.5f }, .normal = zth::math::world_left },
        zth::StandardVertex{ .position = glm::vec3{ 0.5f, 0.0f, -0.5f }, .normal = zth::math::world_left },
        zth::StandardVertex{ .position = glm::vec3{ 0.5f, 0.0f, -1.0f }, .normal = zth::math::world_left },
        zth::StandardVertex{ .position = glm::vec3{ 0.5f, 0.5f, -1.0f }, .normal = zth::math::world_left },
    };

    // --- Camera ---
    _camera.transform().translate(glm::vec3{ 0.0f, 5.0f, 5.0f });
    _camera.emplace_or_replace<zth::ScriptComponent>(zth::make_unique<scripts::Camera>());

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

    // --- Test Mesh ---
    std::shared_ptr<zth::Mesh> test_mesh = std::make_shared<zth::QuadMesh<>>(test_quad_mesh);
    _test_mesh.transform().translate(glm::vec3{ 0.0f, 5.0f, 0.0f });
    _test_mesh.emplace_or_replace<zth::MaterialComponent>(zth::materials::red_plastic());
    _test_mesh.emplace_or_replace<zth::MeshRendererComponent>(test_mesh);
    _test_mesh.emplace_or_replace<zth::MeshColliderComponent>(
        zth::MeshColliderComponent{ .body_id = zth::physics::BodyId(), .mesh = test_mesh });

    // --- Ball Spawner ---
    _ball_spawner.emplace_or_replace<zth::ScriptComponent>(zth::make_unique<scripts::BallSpawner>());

    // --- Directional Light ---
    _directional_light.transform().set_direction(glm::normalize(glm::vec3{ 0.0f, -1.0f, -1.0f }));
    _directional_light.emplace_or_replace<zth::LightComponent>(zth::LightType::Directional);
}
