#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <concepts>

#include "Zenith/Math/Float.hpp"

namespace zth::math {

constexpr auto world_forward = glm::vec3{ 0.0f, 0.0f, -1.0f };
constexpr auto world_right = glm::vec3{ 1.0f, 0.0f, 0.0f };
constexpr auto world_up = glm::vec3{ 0.0f, 1.0f, 0.0f };

template<glm::length_t Size, std::floating_point T>
[[nodiscard]] auto equal(glm::vec<Size, T> a, glm::vec<Size, T> b, T epsilon = default_epsilon<T>) -> bool;

template<glm::length_t Size, std::floating_point T>
[[nodiscard]] auto relative_equal(glm::vec<Size, T> a, glm::vec<Size, T> b, T epsilon = default_epsilon<T>) -> bool;

template<glm::length_t Size, std::floating_point T>
[[nodiscard]] auto has_equal_components(glm::vec<Size, T> vec, T epsilon = default_epsilon<T>) -> bool;

template<glm::length_t Size, std::floating_point T>
[[nodiscard]] auto has_relatively_equal_components(glm::vec<Size, T> vec, T epsilon = default_epsilon<T>) -> bool;

} // namespace zth::math

#include "Vector.inl"
