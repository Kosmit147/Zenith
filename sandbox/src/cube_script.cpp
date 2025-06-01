#include "cube_script.hpp"

namespace scripts {

auto Cube::on_update(zth::EntityHandle actor) -> void
{
    const auto time = static_cast<float>(zth::Application::time());

    auto& transform = actor.get<zth::TransformComponent>();

    transform.rotate(0.0005f * time, glm::normalize(glm::vec3{ 0.0f, 1.0f, 0.0f }));
    transform.rotate(0.0005f * time, glm::normalize(glm::vec3{ -0.3f, 0.1f, 0.7f }));
}

} // namespace scripts
