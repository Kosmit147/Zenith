#include "zenith/graphics/shapes/pyramid_shape.hpp"

#include "zenith/graphics/meshes.hpp"

namespace zth {

auto PyramidShape::mesh() const -> const Mesh&
{
    return meshes::pyramid_mesh();
}

} // namespace zth
