#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <spdlog/spdlog.h>

#include <string>

template<typename T> struct fmt::formatter<glm::vec<2, T>> : formatter<std::string>
{
    static auto format(const glm::vec<2, T>& vec, format_context& ctx) -> decltype(ctx.out())
    {
        return format_to(ctx.out(), "{{ .x = {}, .y = {} }}", vec.x, vec.y);
    }
};

template<typename T> struct fmt::formatter<glm::vec<3, T>> : formatter<std::string>
{
    static auto format(const glm::vec<3, T>& vec, format_context& ctx) -> decltype(ctx.out())
    {
        return format_to(ctx.out(), "{{ .x = {}, .y = {}, .z = {} }}", vec.x, vec.y, vec.z);
    }
};

template<typename T> struct fmt::formatter<glm::vec<4, T>> : formatter<std::string>
{
    static auto format(const glm::vec<4, T>& vec, format_context& ctx) -> decltype(ctx.out())
    {
        return format_to(ctx.out(), "{{ .x = {}, .y = {}, .z = {}, .w = {} }}", vec.x, vec.y, vec.z, vec.w);
    }
};
