#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <spdlog/spdlog.h>

#include <string>

template<> struct fmt::formatter<glm::vec2> : formatter<std::string>
{
    static auto format(const glm::vec2& vec, format_context& ctx) -> decltype(ctx.out())
    {
        return format_to(ctx.out(), "{{ .x = {}, .y = {} }}", vec.x, vec.y);
    }
};

template<> struct fmt::formatter<glm::vec3> : formatter<std::string>
{
    static auto format(const glm::vec3& vec, format_context& ctx) -> decltype(ctx.out())
    {
        return format_to(ctx.out(), "{{ .x = {}, .y = {}, .z = {} }}", vec.x, vec.y, vec.z);
    }
};

template<> struct fmt::formatter<glm::vec4> : formatter<std::string>
{
    static auto format(const glm::vec4& vec, format_context& ctx) -> decltype(ctx.out())
    {
        return format_to(ctx.out(), "{{ .x = {}, .y = {}, .z = {}, .w = {} }}", vec.x, vec.y, vec.z, vec.w);
    }
};
