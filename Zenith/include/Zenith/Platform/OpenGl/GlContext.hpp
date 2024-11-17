#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <utility>

#include "Zenith/Core/Assert.hpp"
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

[[nodiscard]] constexpr auto to_gl_enum(GlProfile profile) -> GLenum
{
    switch (profile)
    {
        using enum GlProfile;
    case Compatibility:
        return GLFW_OPENGL_COMPAT_PROFILE;
    case Core:
        return GLFW_OPENGL_CORE_PROFILE;
    }

    ZTH_ASSERT(false);
    std::unreachable();
}

} // namespace zth
