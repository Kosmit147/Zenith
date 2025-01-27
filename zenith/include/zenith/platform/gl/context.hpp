#pragma once

#include <spdlog/spdlog.h>
#include <string>

#include "zenith/core/typedefs.hpp"

namespace zth {

struct GlVersion
{
    u32 major;
    u32 minor;

    [[nodiscard]] auto operator<=>(const GlVersion&) const = default;
};

enum class GlProfile : u8
{
    Compatibility,
    Core,
};

[[nodiscard]] auto to_string(GlProfile gl_profile) -> const char*;

} // namespace zth

template<> struct fmt::formatter<zth::GlVersion> : formatter<std::string>
{
    static auto format(const zth::GlVersion& version, format_context& ctx) -> decltype(ctx.out())
    {
        auto& [major, minor] = version;
        return format_to(ctx.out(), "{}.{}", major, minor);
    }
};

template<> struct fmt::formatter<zth::GlProfile> : formatter<std::string>
{
    static auto format(zth::GlProfile profile, format_context& ctx) -> decltype(ctx.out())
    {
        return format_to(ctx.out(), "{}", zth::to_string(profile));
    }
};
