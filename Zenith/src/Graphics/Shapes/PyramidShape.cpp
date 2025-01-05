#include "Zenith/Graphics/Shapes/PyramidShape.hpp"

#include "Zenith/Graphics/Meshes.hpp"

namespace zth {

auto PyramidShape::mesh() const -> const Mesh&
{
    return meshes::pyramid_mesh();
}

} // namespace zth
