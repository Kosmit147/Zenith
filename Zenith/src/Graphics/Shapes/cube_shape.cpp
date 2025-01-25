#include "zenith/graphics/shapes/cube_shape.hpp"

#include "zenith/graphics/meshes.hpp"

namespace zth {

auto CubeShape::mesh() const -> const Mesh&
{
    return meshes::cube_mesh();
}

} // namespace zth
