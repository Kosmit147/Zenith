#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>

#include "zenith/math/vector.hpp"

namespace zth::math {

struct Rotation
{
    float angle = 0.0f; // in radians
    glm::vec3 axis = world_up;
};

struct EulerAngles
{
    float pitch = 0.0f; // in radians
    float yaw = 0.0f;   // in radians
    float roll = 0.0f;  // in radians
};

[[nodiscard]] auto to_quaternion(float angle, glm::vec3 axis) -> glm::quat; // axis should be normalized
[[nodiscard]] auto to_quaternion(Rotation rotation) -> glm::quat;           // axis should be normalized
[[nodiscard]] auto to_quaternion(EulerAngles angles) -> glm::quat;

[[nodiscard]] auto to_rotation(glm::vec3 direction) -> Rotation; // direction should be normalized
[[nodiscard]] auto to_direction(glm::quat rotation) -> glm::vec3;
[[nodiscard]] auto to_direction(EulerAngles angles) -> glm::vec3;
[[nodiscard]] auto to_euler_angles(glm::quat rotation) -> EulerAngles;
[[nodiscard]] auto to_euler_angles(glm::vec3 direction) -> EulerAngles; // direction should be normalized

[[nodiscard]] auto rotate(glm::quat quaternion,
                          glm::quat rotation) -> glm::quat; // quaternion and rotation should be normalized
[[nodiscard]] auto rotate(glm::quat quaternion, float angle,
                          glm::vec3 axis) -> glm::quat; // axis should be normalized
[[nodiscard]] auto rotate(glm::quat quaternion, Rotation rotation) -> glm::quat;
[[nodiscard]] auto rotate(glm::quat quaternion, EulerAngles rotation) -> glm::quat;

} // namespace zth::math
