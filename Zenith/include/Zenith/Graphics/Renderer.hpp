#pragma once

#include <glm/vec4.hpp>

#include "Zenith/Platform/OpenGl/GlBuffer.hpp"
#include "Zenith/Platform/OpenGl/VertexArray.hpp"

namespace zth {

class Event;

class Renderer
{
public:
    Renderer() = delete;

    static auto init() -> void;
    static auto on_window_event(const Event& event) -> void;
    static auto shut_down() -> void;

    static auto set_clear_color(glm::vec4 color) -> void;
    static auto clear() -> void;

    static auto draw(const VertexBuffer& vertex_buffer) -> void;
    static auto draw(const VertexArray& vertex_array) -> void;

private:
    static auto log_gl_version() -> void;
};

} // namespace zth
