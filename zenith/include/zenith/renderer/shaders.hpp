#pragma once

#include <array>
#include <memory>

#include "zenith/core/typedefs.hpp"
#include "zenith/gl/fwd.hpp"

namespace zth::shaders {

constexpr usize fallback_shader_index = 0;
constexpr usize flat_color_shader_index = 1;
constexpr usize standard_shader_index = 2;

using ShadersArray = std::array<std::shared_ptr<const gl::Shader>, standard_shader_index + 1>;

auto load() -> void;
auto unload() -> void;

[[nodiscard]] auto all() -> const ShadersArray&;

[[nodiscard]] auto fallback() -> const std::shared_ptr<const gl::Shader>&;
[[nodiscard]] auto flat_color() -> const std::shared_ptr<const gl::Shader>&;
[[nodiscard]] auto standard() -> const std::shared_ptr<const gl::Shader>&;

} // namespace zth::shaders
