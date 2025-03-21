#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "zenith/log/format.hpp"
#include "zenith/stl/string.hpp"

template<typename T> struct fmt::formatter<glm::vec<2, T>> : formatter<zth::String>
{
    static auto format(const glm::vec<2, T>& vec, format_context& ctx) -> decltype(ctx.out())
    {
        return format_to(ctx.out(), "Vec2{{ {}, {} }}", vec.x, vec.y);
    }
};

template<typename T> struct fmt::formatter<glm::vec<3, T>> : formatter<zth::String>
{
    static auto format(const glm::vec<3, T>& vec, format_context& ctx) -> decltype(ctx.out())
    {
        return format_to(ctx.out(), "Vec3{{ {}, {}, {} }}", vec.x, vec.y, vec.z);
    }
};

template<typename T> struct fmt::formatter<glm::vec<4, T>> : formatter<zth::String>
{
    static auto format(const glm::vec<4, T>& vec, format_context& ctx) -> decltype(ctx.out())
    {
        return format_to(ctx.out(), "Vec4{{ {}, {}, {}, {} }}", vec.x, vec.y, vec.z, vec.w);
    }
};

template<typename T> struct fmt::formatter<glm::qua<T>> : formatter<zth::String>
{
    static auto format(const glm::qua<T>& quat, format_context& ctx) -> decltype(ctx.out())
    {
        return format_to(ctx.out(), "Quaternion({} + {}i + {}j + {}k)", quat.w, quat.x, quat.y, quat.z);
    }
};

ZTH_DECLARE_FORMATTER(glm::mat3x3);
ZTH_DECLARE_FORMATTER(glm::mat4x4);
