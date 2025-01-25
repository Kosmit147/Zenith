#include "zenith/graphics/shapes/sphere_shape.hpp"

#include "zenith/graphics/meshes.hpp"

namespace zth {

auto SphereShape::mesh() const -> const Mesh&
{
    return meshes::sphere_mesh();
}

} // namespace zth
