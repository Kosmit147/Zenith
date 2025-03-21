#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "zenith/math/float.hpp"

namespace zth::math {

struct TransformComponents
{
    glm::vec3 translation{ 0.0f };
    glm::quat rotation{ glm::identity<glm::quat>() };
    glm::vec3 scale{ 1.0f };
};

[[nodiscard]] auto compose_transform(glm::vec3 scale, glm::quat rotation, glm::vec3 translation) -> glm::mat4;
[[nodiscard]] auto compose_transform(const TransformComponents& components) -> glm::mat4;
[[nodiscard]] auto decompose_transform(const glm::mat4& transform) -> TransformComponents;

[[nodiscard]] auto get_translation(const glm::mat4& transform) -> glm::vec3;
[[nodiscard]] auto get_scale(const glm::mat4& transform) -> glm::vec3;

[[nodiscard]] auto has_uniform_scale(const glm::mat4& transform, float epsilon = default_epsilon<float>) -> bool;
[[nodiscard]] auto get_normal_matrix(const glm::mat4& transform) -> glm::mat3;

} // namespace zth::math
