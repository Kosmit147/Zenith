#pragma once

namespace zth::math {

template<glm::length_t Size, std::floating_point T>
    requires(Size > 0)
[[nodiscard]] auto max_component(const glm::vec<Size, T>& vec) -> T
{
    auto max = vec[0];

    for (glm::length_t i = 1; i < Size; i++)
        if (vec[i] > max)
            max = vec[i];

    return max;
}

template<glm::length_t Size, std::floating_point T>
    requires(Size > 0)
[[nodiscard]] auto min_component(const glm::vec<Size, T>& vec) -> T
{
    auto min = vec[0];

    for (glm::length_t i = 1; i < Size; i++)
        if (vec[i] < min)
            min = vec[i];

    return min;
}

template<glm::length_t Size, std::floating_point T>
auto equal(const glm::vec<Size, T>& a, const glm::vec<Size, T>& b, T epsilon) -> bool
{
    for (glm::length_t i = 0; i < Size; i++)
        if (!equal(a[i], b[i], epsilon))
            return false;

    return true;
}

template<glm::length_t Size, std::floating_point T>
auto relative_equal(const glm::vec<Size, T>& a, const glm::vec<Size, T>& b, T epsilon) -> bool
{
    for (glm::length_t i = 0; i < Size; i++)
        if (!relative_equal(a[i], b[i], epsilon))
            return false;

    return true;
}

template<glm::length_t Size, std::floating_point T>
    requires(Size > 0)
auto has_equal_components(const glm::vec<Size, T>& vec, T epsilon) -> bool
{
    for (glm::length_t i = 0; i < Size - 1; i++)
        if (!equal(vec[i], vec[i + 1], epsilon))
            return false;

    return true;
}

template<glm::length_t Size, std::floating_point T>
    requires(Size > 0)
auto has_relatively_equal_components(const glm::vec<Size, T>& vec, T epsilon) -> bool
{
    for (glm::length_t i = 0; i < Size - 1; i++)
        if (!relative_equal(vec[i], vec[i + 1], epsilon))
            return false;

    return true;
}

} // namespace zth::math
