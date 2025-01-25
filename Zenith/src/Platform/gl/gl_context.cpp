#include "zenith/platform/gl/gl_context.hpp"

#include "zenith/core/assert.hpp"

namespace zth {

auto to_string(GlProfile gl_profile) -> const char*
{
    switch (gl_profile)
    {
        using enum GlProfile;
    case Compatibility:
        return "Compatibility";
    case Core:
        return "Core";
    }

    ZTH_ASSERT(false);
    return "Unknown";
}

} // namespace zth
