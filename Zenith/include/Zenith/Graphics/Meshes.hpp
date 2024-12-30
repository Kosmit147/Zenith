#pragma once

#include "Zenith/Graphics/Mesh.hpp"
#include "Zenith/Platform/OpenGl/fwd.hpp"
#include "Zenith/Utility/Macros.hpp"

namespace zth::meshes {

struct MeshList
{
    Mesh cube_mesh;
    Mesh sphere_mesh;

    explicit MeshList(const InstanceBuffer& instance_buffer);
    ZTH_NO_COPY_NO_MOVE(MeshList)
    ~MeshList() = default;
};

auto load_meshes(const InstanceBuffer& instance_buffer) -> void;
auto unload_meshes() -> void;

[[nodiscard]] auto meshes() -> const MeshList&;

[[nodiscard]] auto cube_mesh() -> const Mesh&;
[[nodiscard]] auto sphere_mesh() -> const Mesh&;

} // namespace zth::meshes
