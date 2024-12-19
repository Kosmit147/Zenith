#pragma once

#include <glm/fwd.hpp>

#include "Zenith/Graphics/fwd.hpp"
#include "Zenith/Platform/OpenGl/fwd.hpp"

namespace zth {

struct DrawCommand
{
    const VertexArray* vertex_array;
    const Material* material;
    const glm::mat4* transform;

    // comparison operators are used to sort draw commands into batches
    [[nodiscard]] auto operator==(const DrawCommand& other) const -> bool;
    [[nodiscard]] auto operator<(const DrawCommand& other) const -> bool;
    [[nodiscard]] auto operator>(const DrawCommand& other) const -> bool;
    [[nodiscard]] auto operator<=(const DrawCommand& other) const -> bool;
    [[nodiscard]] auto operator>=(const DrawCommand& other) const -> bool;
};

} // namespace zth
