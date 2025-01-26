#pragma once

#include <concepts>
#include <limits>

namespace zth::math {

template<std::floating_point T> constexpr auto default_epsilon = std::numeric_limits<T>::epsilon();

template<std::floating_point T> [[nodiscard]] auto equal(T a, T b, T epsilon = default_epsilon<T>) -> bool;
template<std::floating_point T> [[nodiscard]] auto relative_equal(T a, T b, T epsilon = default_epsilon<T>) -> bool;

} // namespace zth::math

#include "float.inl"
