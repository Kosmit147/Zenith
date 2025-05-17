#pragma once

#include <glm/gtx/structured_bindings.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <algorithm>

#include "zenith/core/typedefs.hpp"

namespace zth {

[[nodiscard]] constexpr auto normalize_rgb8(glm::vec<3, u8> color) -> glm::vec3
{
    return glm::vec3{ color } / 255.0f;
}

[[nodiscard]] constexpr auto normalize_rgba8(glm::vec<4, u8> color) -> glm::vec4
{
    return glm::vec4{ color } / 255.0f;
}

[[nodiscard]] constexpr auto normalize_rgb8(u8 r, u8 g, u8 b) -> glm::vec3
{
    return normalize_rgb8(glm::vec<3, u8>{ r, g, b });
}

[[nodiscard]] constexpr auto normalize_rgba8(u8 r, u8 g, u8 b, u8 a) -> glm::vec4
{
    return normalize_rgba8(glm::vec<4, u8>{ r, g, b, a });
}

[[nodiscard]] constexpr auto saturate_color(glm::vec3 color) -> glm::vec3
{
    auto [r, g, b] = color;
    auto max = std::max(r, std::max(g, b));
    auto scale = max != 0.0f ? 1.0f / max : 1.0f;
    return color * scale;
}

// Leaves alpha unchanged.
[[nodiscard]] constexpr auto saturate_color(glm::vec4 color) -> glm::vec4
{
    return { saturate_color(glm::vec3{ color }), color.a };
}

[[nodiscard]] constexpr auto saturate_color(glm::vec<3, u8> color) -> glm::vec<3, u8>
{
    auto f_color = glm::vec3{ color };
    auto [r, g, b] = f_color;
    auto max = std::max(r, std::max(g, b));
    auto scale = max != 0.0f ? 255.0f / max : 1.0f;
    return f_color * scale;
}

// Leaves alpha unchanged.
[[nodiscard]] constexpr auto saturate_color(glm::vec<4, u8> color) -> glm::vec<4, u8>
{
    return { saturate_color(glm::vec<3, u8>{ color }), color.a };
}

} // namespace zth
