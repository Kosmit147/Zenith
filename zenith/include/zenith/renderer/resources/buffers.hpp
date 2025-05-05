#pragma once

#include "zenith/core/typedefs.hpp"
#include "zenith/gl/fwd.hpp"
#include "zenith/renderer/quad.hpp"

namespace zth::buffers {

constexpr inline usize quads_index_buffer_quads =
    16384; // The biggest number of quads that still allows us to use unsigned short for indexing.
constexpr inline usize quads_index_buffer_indices = quads_index_buffer_quads * indices_per_quad;

auto create() -> void;
auto destroy() -> void;

[[nodiscard]] auto scratch() -> gl::Buffer&;
[[nodiscard]] auto quads_index_buffer() -> gl::IndexBuffer&;

} // namespace zth::buffers
