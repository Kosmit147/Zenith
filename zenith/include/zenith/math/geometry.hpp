#pragma once

#include <glm/vec2.hpp>

#include <array>

#include "zenith/math/fwd.hpp"

namespace zth {

template<typename T /* = float */> struct Rect
{
    glm::vec<2, T> position;
    glm::vec<2, T> size;

    [[nodiscard]] constexpr auto top_left() const { return position; }
    [[nodiscard]] constexpr auto bottom_left() const { return position + glm::vec<2, T>{ static_cast<T>(0), -size.y }; }
    [[nodiscard]] constexpr auto bottom_right() const { return position + glm::vec<2, T>{ size.x, -size.y }; }
    [[nodiscard]] constexpr auto top_right() const { return position + glm::vec<2, T>{ size.x, static_cast<T>(0) }; }

    [[nodiscard]] constexpr auto vertices() const -> std::array<glm::vec<2, T>, 4>
    {
        return { top_left(), bottom_left(), bottom_right(), top_right() };
    }

    [[nodiscard]] constexpr operator BoundedRect<T>() const;
};

template<typename T /* = float */> struct BoundedRect
{
    glm::vec<2, T> top_left;
    glm::vec<2, T> bottom_right;

    [[nodiscard]] constexpr auto bottom_left() const { return glm::vec<2, T>{ top_left.x, bottom_right.y }; }
    [[nodiscard]] constexpr auto top_right() const { return glm::vec<2, T>{ bottom_right.x, top_left.y }; }

    [[nodiscard]] constexpr auto size() const
    {
        return glm::vec<2, T>{ bottom_right.x - top_left.x, top_left.y - bottom_right.y };
    }

    [[nodiscard]] constexpr auto vertices() const -> std::array<glm::vec<2, T>, 4>
    {
        return { top_left, bottom_left(), bottom_right, top_right() };
    }

    [[nodiscard]] constexpr operator Rect<T>() const;
};

template<typename T> [[nodiscard]] constexpr Rect<T>::operator BoundedRect<T>() const
{
    return BoundedRect{
        .top_left = top_left(),
        .bottom_right = bottom_right(),
    };
}

template<typename T> [[nodiscard]] constexpr BoundedRect<T>::operator Rect<T>() const
{
    return Rect{ .position = top_left, .size = size() };
}

} // namespace zth
