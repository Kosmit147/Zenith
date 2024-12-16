#pragma once

#include "Zenith/Graphics/fwd.hpp"
#include "Zenith/Platform/OpenGl/fwd.hpp"

namespace zth {

class Transformable3D;

struct DrawCommand
{
    const VertexArray* vertex_array;
    const Material* material;
    const Transformable3D* transform;

    // comparison operators are used to sort draw commands into batches
    auto operator==(const DrawCommand& other) const -> bool;
    auto operator<(const DrawCommand& other) const -> bool;
    auto operator>(const DrawCommand& other) const -> bool;
    auto operator<=(const DrawCommand& other) const -> bool;
    auto operator>=(const DrawCommand& other) const -> bool;
};

} // namespace zth
