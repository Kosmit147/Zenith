#include "Zenith/Graphics/Shapes/CubeShape.hpp"

#include "Zenith/Graphics/Meshes.hpp"

namespace zth {

auto CubeShape::mesh() const -> const Mesh&
{
    return meshes::cube_mesh();
}

} // namespace zth
