#pragma once

#include "Zenith/Graphics/Shapes/Shape.hpp"

namespace zth {

class CubeShape : public Shape3D
{
public:
    using Shape3D::Shape3D;

    [[nodiscard]] auto mesh() const -> const Mesh& override;
    auto draw() const -> void override;
};

} // namespace zth
