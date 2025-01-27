#pragma once

#include <spdlog/fmt/fmt.h>

#include <string>

#include "zenith/core/typedefs.hpp"

namespace zth {

namespace gl {

struct Version
{
    u32 major;
    u32 minor;

    [[nodiscard]] auto operator<=>(const Version&) const = default;
};

enum class Profile : u8
{
    Compatibility,
    Core,
};

auto set_debug_context() -> void;

} // namespace gl

[[nodiscard]] auto to_string(gl::Profile gl_profile) -> const char*;

} // namespace zth

template<> struct fmt::formatter<zth::gl::Version> : formatter<std::string>
{
    static auto format(const zth::gl::Version& version, format_context& ctx) -> decltype(ctx.out())
    {
        auto& [major, minor] = version;
        return format_to(ctx.out(), "{}.{}", major, minor);
    }
};

template<> struct fmt::formatter<zth::gl::Profile> : formatter<std::string>
{
    static auto format(zth::gl::Profile profile, format_context& ctx) -> decltype(ctx.out())
    {
        return format_to(ctx.out(), "{}", zth::to_string(profile));
    }
};
