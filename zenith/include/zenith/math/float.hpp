#pragma once

#include <algorithm>
#include <cmath>
#include <concepts>
#include <limits>

namespace zth::math {

template<std::floating_point T> constexpr auto default_epsilon = std::numeric_limits<T>::epsilon();

template<std::floating_point T> [[nodiscard]] auto equal(T a, T b, T epsilon = default_epsilon<T>) -> bool
{
    return std::abs(a - b) <= epsilon;
}

template<std::floating_point T> [[nodiscard]] auto relative_equal(T a, T b, T epsilon = default_epsilon<T>) -> bool
{
    return std::abs(a - b) <= epsilon * std::max(std::abs(a), std::abs(b));
}

} // namespace zth::math
