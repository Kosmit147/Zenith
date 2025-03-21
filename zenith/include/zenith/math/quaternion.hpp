#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>

#include "zenith/math/vector.hpp"

namespace zth::math {

struct Rotation
{
    float angle = 0.0f; // In radians.
    glm::vec3 axis = world_up;
};

struct EulerAngles
{
    float pitch = 0.0f; // In radians.
    float yaw = 0.0f;   // In radians.
    float roll = 0.0f;  // In radians.

    [[nodiscard]] auto operator+(const EulerAngles& other) const -> EulerAngles;
    [[nodiscard]] auto operator-(const EulerAngles& other) const -> EulerAngles;

    auto operator+=(const EulerAngles& other) -> EulerAngles&;
    auto operator-=(const EulerAngles& other) -> EulerAngles&;

    [[nodiscard]] auto operator-() const -> EulerAngles;
};

[[nodiscard]] auto to_quaternion(float angle, glm::vec3 axis) -> glm::quat; // axis must be normalized.
[[nodiscard]] auto to_quaternion(glm::vec3 direction) -> glm::quat;         // direction must be normalized.
[[nodiscard]] auto to_quaternion(Rotation rotation) -> glm::quat;           // axis must be normalized.
[[nodiscard]] auto to_quaternion(EulerAngles angles) -> glm::quat;

[[nodiscard]] auto to_rotation(glm::vec3 direction) -> Rotation; // direction must be normalized.
[[nodiscard]] auto to_direction(glm::quat rotation) -> glm::vec3;
[[nodiscard]] auto to_direction(EulerAngles angles) -> glm::vec3;
[[nodiscard]] auto to_euler_angles(glm::vec3 direction) -> EulerAngles; // direction must be normalized.
[[nodiscard]] auto to_euler_angles(glm::quat rotation) -> EulerAngles;

[[nodiscard]] auto rotate(glm::quat quaternion, glm::quat rotation)
    -> glm::quat; // quaternion and rotation must be normalized.
[[nodiscard]] auto rotate(glm::quat quaternion, float angle, glm::vec3 axis) -> glm::quat; // axis must be normalized.
[[nodiscard]] auto rotate(glm::quat quaternion, Rotation rotation) -> glm::quat;
[[nodiscard]] auto rotate(glm::quat quaternion, EulerAngles rotation) -> glm::quat;

} // namespace zth::math
