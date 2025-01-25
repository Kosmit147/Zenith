#pragma once

#include <glm/fwd.hpp>
#include <glm/vec3.hpp>

#include "zenith/core/transformable.hpp"
#include "zenith/graphics/fwd.hpp"
#include "zenith/utility/macros.hpp"

namespace zth {

class Shape3D : public Transformable3D
{
public:
    explicit Shape3D() = default;
    explicit Shape3D(glm::vec3 translation);
    explicit Shape3D(const glm::mat4& transform);
    ZTH_DEFAULT_COPY_DEFAULT_MOVE(Shape3D)
    ~Shape3D() override = default;

    [[nodiscard]] virtual auto mesh() const -> const Mesh& = 0;
};

} // namespace zth
