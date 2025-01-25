#pragma once

#include "zenith/graphics/shapes/shape.hpp"

namespace zth {

class SphereShape : public Shape3D
{
public:
    using Shape3D::Shape3D;

    [[nodiscard]] auto mesh() const -> const Mesh& override;
};

} // namespace zth
