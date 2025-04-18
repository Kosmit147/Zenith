#pragma once

#include "zenith/gl/fwd.hpp"

namespace zth::textures {

auto create() -> void;
auto destroy() -> void;

[[nodiscard]] auto white() -> const std::shared_ptr<const gl::Texture2D>&;
[[nodiscard]] auto black() -> const std::shared_ptr<const gl::Texture2D>&;
[[nodiscard]] auto transparent() -> const std::shared_ptr<const gl::Texture2D>&;

} // namespace zth::textures
