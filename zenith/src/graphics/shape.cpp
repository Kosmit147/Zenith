#include "zenith/graphics/shape.hpp"

#include "zenith/graphics/meshes.hpp"

namespace zth {

Shape3D::Shape3D(glm::vec3 translation) : Transformable3D(translation) {}

Shape3D::Shape3D(const glm::mat4& transform) : Transformable3D(transform) {}

auto CubeShape::mesh() const -> const Mesh&
{
    return meshes::cube_mesh();
}

auto PyramidShape::mesh() const -> const Mesh&
{
    return meshes::pyramid_mesh();
}

auto SphereShape::mesh() const -> const Mesh&
{
    return meshes::sphere_mesh();
}

} // namespace zth
