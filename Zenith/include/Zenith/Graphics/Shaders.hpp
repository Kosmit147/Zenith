#pragma once

#include "Zenith/Platform/OpenGl/Shader.hpp"

namespace zth::shaders {

auto load_shaders() -> void;
auto unload_shaders() -> void;

// TODO: these should be non-modifiable
inline const Shader* flat_color;
inline const Shader* standard;

} // namespace zth::shaders
