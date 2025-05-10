#pragma once

#include "zenith/stl/string.hpp"

// @refactor: Use #embed from C23 once MSVC supports it.

namespace zth::embedded::shaders {

extern const StringView defines_glsl;

extern const StringView fallback_vert;
extern const StringView fallback_frag;
extern const StringView flat_color_vert;
extern const StringView flat_color_frag;
extern const StringView standard_vert;
extern const StringView standard_frag;
extern const StringView texture_2d_vert;
extern const StringView texture_2d_frag;

} // namespace zth::embedded::shaders
