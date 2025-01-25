#pragma once

#include <algorithm>
#include <cmath>

namespace zth::math {

template<std::floating_point T> auto equal(T a, T b, T epsilon) -> bool
{
    return std::abs(a - b) <= epsilon;
}

template<std::floating_point T> auto relative_equal(T a, T b, T epsilon) -> bool
{
    return std::abs(a - b) <= epsilon * std::max(std::abs(a), std::abs(b));
}

} // namespace zth::math
