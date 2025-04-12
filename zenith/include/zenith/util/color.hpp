#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "zenith/core/typedefs.hpp"

namespace zth {

[[nodiscard]] constexpr auto normalize_rgb8(u8 r, u8 g, u8 b) -> glm::vec3
{
    return glm::vec3{ static_cast<float>(r) / 255.0f, static_cast<float>(g) / 255.0f, static_cast<float>(b) / 255.0f };
}

[[nodiscard]] constexpr auto normalize_rgba8(u8 r, u8 g, u8 b, u8 a) -> glm::vec4
{
    return glm::vec4{
        normalize_rgb8(r, g, b),
        static_cast<float>(a) / 255.0f,
    };
}

} // namespace zth
