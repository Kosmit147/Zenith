#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace zth {

class Transformable
{
public:
    auto translate(glm::vec3 translation) -> Transformable&;
    auto rotate(float angle, glm::vec3 axis) -> Transformable&;
    auto scale(glm::vec3 factor) -> Transformable&;

    auto set_transform(const glm::mat4& transform) -> void;
    [[nodiscard]] auto transform() const -> auto& { return _transform; }

private:
    glm::vec3 _translation{ 0.0f };
    glm::quat _rotation{};
    glm::vec3 _scale{ 1.0f };

    glm::mat4 _transform{ 1.0f };

private:
    auto update_transform() -> void;
};

} // namespace zth
