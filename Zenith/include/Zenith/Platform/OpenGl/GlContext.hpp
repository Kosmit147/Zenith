#pragma once

#include <spdlog/spdlog.h>
#include <string>

#include "Zenith/Core/Typedefs.hpp"

namespace zth {

struct GlVersion
{
    u32 major;
    u32 minor;

    auto operator<=>(const GlVersion&) const = default;
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
        return format_to(ctx.out(), "{}.{}", version.major, version.minor);
    }
};

template<> struct fmt::formatter<zth::GlProfile> : formatter<std::string>
{
    static auto format(zth::GlProfile profile, format_context& ctx) -> decltype(ctx.out())
    {
        return format_to(ctx.out(), "{}", zth::to_string(profile));
    }
};
