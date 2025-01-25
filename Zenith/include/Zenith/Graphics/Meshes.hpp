#pragma once

#include "zenith/graphics/mesh.hpp"
#include "zenith/platform/gl/fwd.hpp"
#include "zenith/utility/macros.hpp"

namespace zth::meshes {

struct MeshList
{
    Mesh cube_mesh;
    Mesh pyramid_mesh;
    Mesh sphere_mesh;

    explicit MeshList(const InstanceBuffer& instance_buffer);
    ZTH_NO_COPY_NO_MOVE(MeshList)
    ~MeshList() = default;
};

auto load_meshes(const InstanceBuffer& instance_buffer) -> void;
auto unload_meshes() -> void;

[[nodiscard]] auto meshes() -> const MeshList&;

[[nodiscard]] auto cube_mesh() -> const Mesh&;
[[nodiscard]] auto pyramid_mesh() -> const Mesh&;
[[nodiscard]] auto sphere_mesh() -> const Mesh&;

} // namespace zth::meshes
