#pragma once

#include <algorithm>

namespace zth::math {

template<glm::length_t Size, typename T>
    requires(Size > 0)
auto max_component(const glm::vec<Size, T>& vec) -> T
{
    auto max = vec[0];

    for (glm::length_t i = 1; i < Size; i++)
        max = std::max(max, vec[i]);

    return max;
}

template<glm::length_t Size, typename T>
    requires(Size > 0)
auto min_component(const glm::vec<Size, T>& vec) -> T
{
    auto min = vec[0];

    for (glm::length_t i = 1; i < Size; i++)
        min = std::min(min, vec[i]);

    return min;
}

template<glm::length_t Size, typename T>
    requires(Size > 0)
auto min_max_component(const glm::vec<Size, T>& vec) -> std::pair<T, T>
{
    auto min = vec[0];
    auto max = vec[0];

    for (glm::length_t i = 1; i < Size; i++)
    {
        max = std::max(max, vec[i]);
        min = std::min(min, vec[i]);
    }

    return { min, max };
}

template<glm::length_t Size, std::floating_point T>
auto float_equal(const glm::vec<Size, T>& a, const glm::vec<Size, T>& b, T epsilon) -> bool
{
    for (glm::length_t i = 0; i < Size; i++)
        if (!float_equal(a[i], b[i], epsilon))
            return false;

    return true;
}

template<glm::length_t Size, std::floating_point T>
auto relative_float_equal(const glm::vec<Size, T>& a, const glm::vec<Size, T>& b, T epsilon) -> bool
{
    for (glm::length_t i = 0; i < Size; i++)
        if (!relative_float_equal(a[i], b[i], epsilon))
            return false;

    return true;
}

template<glm::length_t Size, std::floating_point T>
    requires(Size > 0)
auto has_equal_float_components(const glm::vec<Size, T>& vec, T epsilon) -> bool
{
    for (glm::length_t i = 0; i < Size - 1; i++)
        if (!float_equal(vec[i], vec[i + 1], epsilon))
            return false;

    return true;
}

template<glm::length_t Size, std::floating_point T>
    requires(Size > 0)
auto has_relatively_equal_float_components(const glm::vec<Size, T>& vec, T epsilon) -> bool
{
    for (glm::length_t i = 0; i < Size - 1; i++)
        if (!relative_float_equal(vec[i], vec[i + 1], epsilon))
            return false;

    return true;
}

} // namespace zth::math
