#pragma once

#include "Zenith/Core/Transformable.hpp"
#include "Zenith/Graphics/Drawable.hpp"
#include "Zenith/Graphics/fwd.hpp"
#include "Zenith/Utility/Utility.hpp"

namespace zth {

class Shape : public Drawable
{
public:
    explicit Shape() = default;
    ZTH_DEFAULT_COPY_DEFAULT_MOVE(Shape)
    ~Shape() override = default;

    // TODO: is this needed?
    [[nodiscard]] virtual auto mesh() const -> const Mesh& = 0;
};

class Shape2D : public Shape, public Transformable2D
{
    // TODO
};

class Shape3D : public Shape, public Transformable3D
{
public:
    explicit Shape3D() = default;
    explicit Shape3D(const glm::mat4& transform);
    ZTH_DEFAULT_COPY_DEFAULT_MOVE(Shape3D)
    ~Shape3D() override = default;
};

} // namespace zth
