#include "Zenith/Graphics/Shapes/CubeShape.hpp"

#include "Zenith/Graphics/Meshes.hpp"
#include "Zenith/Graphics/Renderer.hpp"

namespace zth {

auto CubeShape::mesh() const -> const Mesh&
{
    return *meshes::cube_mesh;
}

auto CubeShape::draw() const -> void
{
    Renderer::draw(*this);
}

} // namespace zth
