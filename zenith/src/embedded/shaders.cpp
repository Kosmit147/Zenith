#include "zenith/embedded/shaders.hpp"

#include <battery/embed.hpp>

namespace zth::embedded::shaders {

const StringView defines_glsl = b::embed<"src/shaders/zth_defines.glsl">().str();

const StringView fallback_vert = b::embed<"src/shaders/zth_fallback.vert">().str();
const StringView fallback_frag = b::embed<"src/shaders/zth_fallback.frag">().str();
const StringView flat_color_vert = b::embed<"src/shaders/zth_flat_color.vert">().str();
const StringView flat_color_frag = b::embed<"src/shaders/zth_flat_color.frag">().str();
const StringView standard_vert = b::embed<"src/shaders/zth_standard.vert">().str();
const StringView standard_frag = b::embed<"src/shaders/zth_standard.frag">().str();

} // namespace zth::embedded::shaders
