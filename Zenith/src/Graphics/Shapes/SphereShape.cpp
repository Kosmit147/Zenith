#include "Zenith/Graphics/Shapes/SphereShape.hpp"

#include "Zenith/Graphics/Meshes.hpp"

namespace zth {

auto SphereShape::mesh() const -> const Mesh&
{
    return *meshes::sphere_mesh;
}

} // namespace zth
