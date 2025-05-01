#include "zenith/renderer/light.hpp"

#include "zenith/core/assert.hpp"

namespace zth {

auto to_string(LightType light_type) -> const char*
{
    switch (light_type)
    {
        using enum LightType;
    case Directional:
        return "Directional";
    case Point:
        return "Point";
    case Spot:
        return "Spot";
    case Ambient:
        return "Ambient";
    }

    ZTH_ASSERT(false);
    return "Unknown";
}

} // namespace zth

ZTH_DEFINE_FORMATTER(zth::LightType, light_type)
{
    return ZTH_FORMAT_OUT("{}", zth::to_string(light_type));
}
