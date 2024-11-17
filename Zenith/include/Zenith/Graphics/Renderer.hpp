#pragma once

#include <glad/glad.h>
#include <glm/vec4.hpp>

#include "Zenith/Utility/Utility.hpp"

namespace zth {

class Renderer
{
public:
    Renderer() = default;
    ZTH_NO_COPY_NO_MOVE(Renderer)
    ~Renderer() = default;

    auto set_clear_color(glm::vec4 color) const -> void { glClearColor(color.r, color.g, color.b, color.a); }
    auto clear() const -> void { glClear(GL_COLOR_BUFFER_BIT); }
};

} // namespace zth
