#include "ball_spawner.hpp"

namespace scripts {

auto BallSpawner::display_label() const -> const char*
{
    return "Ball Spawner";
}

auto BallSpawner::debug_edit() -> void
{
    zth::debug::drag_int("Count", count);
    zth::debug::drag_float("Spread", spread);
    zth::debug::drag_float("Min Height", min_height);
    zth::debug::drag_float("Max Height", max_height);

    if (zth::debug::button("Respawn"))
        respawn();
}

auto BallSpawner::on_attach([[maybe_unused]] zth::EntityHandle actor) -> void
{
    respawn();
}

auto BallSpawner::respawn() -> void
{
    for (auto& ball : _balls)
        ball.destroy();

    auto& scene = zth::SceneManager::scene();

    for (std::size_t i = 0; i < count; i++)
    {
        auto ball = scene.create_entity(zth::format("Ball {}", i));

        // @todo: Add functions for getting random vectors to Random.
        glm::vec3 position{
            zth::Random::get_float(-spread, spread),
            zth::Random::get_float(min_height, max_height),
            zth::Random::get_float(-spread, spread),
        };

        ball.transform().translate(position);
        ball.emplace_or_replace<zth::SphereColliderComponent>();
        ball.emplace_or_replace<zth::RigidBodyComponent>();
        ball.emplace_or_replace<zth::MeshRendererComponent>(zth::meshes::sphere());
        ball.emplace_or_replace<zth::MaterialComponent>(zth::materials::plain());

        _balls.push_back(ball);
    }
}

} // namespace scripts
