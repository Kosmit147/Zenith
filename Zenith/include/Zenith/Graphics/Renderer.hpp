#pragma once

#include <glm/vec4.hpp>

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

private:
    static auto log_gl_version() -> void;
};

} // namespace zth
