#pragma once

#include <glm/vec2.hpp>

#include "zenith/math/geometry.hpp"

// Pixel coordinates are defined as:
// X starts at 0 on the left and increases up to maximum width on the right.
// Y starts at 0 on the bottom and increases up to maximum height on the top.

// Normalized device coordinates (ndc) are defined as:
// X starts at -1 on the left and increases up to 1 on the right.
// Y starts at -1 on the bottom and increases up to 1 on the top.

namespace zth {

template<typename T>
[[nodiscard]] constexpr auto pixel_coordinate_to_ndc(glm::vec<2, T> coordinate, glm::uvec2 space) -> glm::vec2
{
    glm::vec2 float_coordinate = coordinate;
    glm::vec2 float_space = space;
    return (float_coordinate / float_space - 0.5f) * 2.0f;
}

template<typename T> [[nodiscard]] constexpr auto pixel_size_to_ndc(glm::vec<2, T> size, glm::uvec2 space) -> glm::vec2
{
    glm::vec2 float_size = size;
    glm::vec2 float_space = space;
    return float_size / float_space * 2.0f;
}

template<typename T>
[[nodiscard]] constexpr auto rect_in_pixel_coordinates_to_rect_in_ndc(const Rect<T>& rect, glm::uvec2 space) -> Rect<>
{
    return Rect{
        .position = pixel_coordinate_to_ndc(rect.position, space),
        .size = pixel_size_to_ndc(rect.size, space),
    };
}

template<typename T>
[[nodiscard]] constexpr auto rect_bounds_in_pixel_coordinates_to_rect_bounds_in_ndc(const Rect<T>& rect,
                                                                                    glm::uvec2 space) -> Rect<>
{
    return RectBounds{
        .top_left = pixel_coordinate_to_ndc(rect.top_left(), space),
        .bottom_right = pixel_coordinate_to_ndc(rect.bottom_right(), space),
    };
}

} // namespace zth
