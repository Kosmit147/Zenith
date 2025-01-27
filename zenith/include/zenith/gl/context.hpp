#pragma once

#include "zenith/core/typedefs.hpp"
#include "zenith/log/format.hpp"

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

ZTH_DECLARE_FORMATTER(zth::gl::Version);
ZTH_DECLARE_FORMATTER(zth::gl::Profile);
