#include "zenith/embedded/shaders.hpp"

#include <battery/embed.hpp>

namespace zth::embedded::shaders {

const std::string_view defines_glsl = b::embed<"src/shaders/zth_defines.glsl">().str();

const std::string_view fallback_vert = b::embed<"src/shaders/zth_fallback.vert">().str();
const std::string_view fallback_frag = b::embed<"src/shaders/zth_fallback.frag">().str();
const std::string_view flat_color_vert = b::embed<"src/shaders/zth_flat_color.vert">().str();
const std::string_view flat_color_frag = b::embed<"src/shaders/zth_flat_color.frag">().str();
const std::string_view standard_vert = b::embed<"src/shaders/zth_standard.vert">().str();
const std::string_view standard_frag = b::embed<"src/shaders/zth_standard.frag">().str();

// @todo: remove test_glsl
const std::string_view test_glsl = b::embed<"src/shaders/test.glsl">().str();

} // namespace zth::embedded::shaders
