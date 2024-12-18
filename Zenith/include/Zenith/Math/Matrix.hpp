#pragma once

#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "Zenith/Math/Float.hpp"

namespace zth::math {

[[nodiscard]] auto get_translation(const glm::mat4& transform) -> glm::vec3;
[[nodiscard]] auto get_scale(const glm::mat4& transform) -> glm::vec3;

[[nodiscard]] auto has_uniform_scale(const glm::mat4& transform, float epsilon = default_epsilon<float>) -> bool;
[[nodiscard]] auto get_normal_matrix(const glm::mat4& transform) -> glm::mat3;

} // namespace zth::math
