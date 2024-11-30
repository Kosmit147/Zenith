#pragma once

#include "Zenith/Core/Typedefs.hpp"

namespace zth {

struct GlVersion
{
    u32 major;
    u32 minor;
};

enum class GlProfile : u8
{
    Compatibility,
    Core,
};

} // namespace zth
