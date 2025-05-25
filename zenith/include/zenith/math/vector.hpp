#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <concepts>

#include "zenith/math/float.hpp"

namespace zth::math {

constexpr inline glm::vec3 world_backward{ 0.0f, 0.0f, 1.0f };
constexpr inline glm::vec3 world_forward{ 0.0f, 0.0f, -1.0f };
constexpr inline glm::vec3 world_left{ -1.0f, 0.0f, 0.0f };
constexpr inline glm::vec3 world_right{ 1.0f, 0.0f, 0.0f };
constexpr inline glm::vec3 world_down{ 0.0f, -1.0f, 0.0f };
constexpr inline glm::vec3 world_up{ 0.0f, 1.0f, 0.0f };

template<glm::length_t Size, std::floating_point T>
    requires(Size > 0)
[[nodiscard]] auto max_component(const glm::vec<Size, T>& vec) -> T;
template<glm::length_t Size, std::floating_point T>
    requires(Size > 0)
[[nodiscard]] auto min_component(const glm::vec<Size, T>& vec) -> T;

template<glm::length_t Size, std::floating_point T>
[[nodiscard]] auto equal(const glm::vec<Size, T>& a, const glm::vec<Size, T>& b, T epsilon = default_epsilon<T>)
    -> bool;

template<glm::length_t Size, std::floating_point T>
[[nodiscard]] auto relative_equal(const glm::vec<Size, T>& a, const glm::vec<Size, T>& b,
                                  T epsilon = default_epsilon<T>) -> bool;

template<glm::length_t Size, std::floating_point T>
    requires(Size > 0)
[[nodiscard]] auto has_equal_components(const glm::vec<Size, T>& vec, T epsilon = default_epsilon<T>) -> bool;

template<glm::length_t Size, std::floating_point T>
    requires(Size > 0)
[[nodiscard]] auto has_relatively_equal_components(const glm::vec<Size, T>& vec, T epsilon = default_epsilon<T>)
    -> bool;

} // namespace zth::math

#include "vector.inl"
