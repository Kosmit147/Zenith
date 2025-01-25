#pragma once

namespace zth::math {

template<glm::length_t Size, std::floating_point T>
auto equal(glm::vec<Size, T> a, glm::vec<Size, T> b, T epsilon) -> bool
{
    for (glm::length_t i = 0; i < Size; i++)
        if (!equal(a[i], b[i], epsilon))
            return false;

    return true;
}

template<glm::length_t Size, std::floating_point T>
auto relative_equal(glm::vec<Size, T> a, glm::vec<Size, T> b, T epsilon) -> bool
{
    for (glm::length_t i = 0; i < Size; i++)
        if (!relative_equal(a[i], b[i], epsilon))
            return false;

    return true;
}

template<glm::length_t Size, std::floating_point T> auto has_equal_components(glm::vec<Size, T> vec, T epsilon) -> bool
{
    static_assert(Size > 0);

    for (glm::length_t i = 0; i < Size - 1; i++)
        if (!equal(vec[i], vec[i + 1], epsilon))
            return false;

    return true;
}

template<glm::length_t Size, std::floating_point T>
auto has_relatively_equal_components(glm::vec<Size, T> vec, T epsilon) -> bool
{
    static_assert(Size > 0);

    for (glm::length_t i = 0; i < Size - 1; i++)
        if (!relative_equal(vec[i], vec[i + 1], epsilon))
            return false;

    return true;
}

} // namespace zth::math
