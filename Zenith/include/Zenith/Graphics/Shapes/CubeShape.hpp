#pragma once

#include "Zenith/Graphics/Shapes/Shape.hpp"

namespace zth {

class CubeShape : public Shape3D
{
public:
    explicit CubeShape() = default;

    [[nodiscard]] auto mesh() const -> const Mesh& override;
    auto draw() const -> void override;
};

} // namespace zth
