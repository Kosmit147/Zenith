#pragma once

#include "Zenith/Graphics/Shapes/Shape.hpp"

namespace zth {

class PyramidShape : public Shape3D
{
public:
    using Shape3D::Shape3D;

    [[nodiscard]] auto mesh() const -> const Mesh& override;
};

} // namespace zth
