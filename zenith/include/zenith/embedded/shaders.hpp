#pragma once

#include <string_view>

namespace zth::embedded::shaders {

extern const std::string_view defines_glsl;

extern const std::string_view fallback_vert;
extern const std::string_view fallback_frag;
extern const std::string_view flat_color_vert;
extern const std::string_view flat_color_frag;
extern const std::string_view standard_vert;
extern const std::string_view standard_frag;

// @todo: remove test_glsl
extern const std::string_view test_glsl;

} // namespace zth::embedded::shaders
