#pragma once

#include "Zenith/Graphics/Mesh.hpp"

namespace zth::meshes {

auto load_meshes() -> void;
auto unload_meshes() -> void;

// TODO: these should be non-modifiable
inline const Mesh* cube_mesh;
inline const Mesh* sphere_mesh;

} // namespace zth::meshes
