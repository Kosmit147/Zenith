#pragma once

#include "zenith/gl/fwd.hpp"

namespace zth::buffers {

auto create() -> void;
auto destroy() -> void;

[[nodiscard]] auto scratch() -> gl::Buffer&;

} // namespace zth::buffers
