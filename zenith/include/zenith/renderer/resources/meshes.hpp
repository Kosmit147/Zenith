#pragma once

#include <array>
#include <memory>

#include "zenith/core/typedefs.hpp"
#include "zenith/renderer/fwd.hpp"

namespace zth::meshes {

constexpr inline usize cube_mesh_index = 0;
constexpr inline usize pyramid_mesh_index = 1;
constexpr inline usize sphere_mesh_index = 2;

using MeshesArray = std::array<std::shared_ptr<const Mesh>, sphere_mesh_index + 1>;

auto load() -> void;
auto unload() -> void;

[[nodiscard]] auto all() -> const MeshesArray&;

[[nodiscard]] auto cube() -> const std::shared_ptr<const Mesh>&;
[[nodiscard]] auto pyramid() -> const std::shared_ptr<const Mesh>&;
[[nodiscard]] auto sphere() -> const std::shared_ptr<const Mesh>&;

} // namespace zth::meshes
