#include "Zenith/Platform/OpenGl/GlContext.hpp"

#include "Zenith/Core/Assert.hpp"

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
