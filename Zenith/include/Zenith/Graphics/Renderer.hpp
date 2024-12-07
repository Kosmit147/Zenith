#pragma once

#include <glm/fwd.hpp>
#include <glm/vec4.hpp>

#include <memory>
#include <vector>

#include "Zenith/Graphics/fwd.hpp"
#include "Zenith/Platform/OpenGl/fwd.hpp"

namespace zth {

class Event;

struct RenderStates
{
    const glm::mat4* transform = nullptr;
    const Shader* shader = nullptr;
    const Texture2D* texture = nullptr;
};

struct DrawCommand
{
    const Drawable* drawable;
    RenderStates render_states;
};

class Renderer
{
public:
    Renderer() = delete;

    static auto init() -> void;
    static auto on_window_event(const Event& event) -> void;
    static auto shut_down() -> void;

    static auto set_clear_color(glm::vec4 color) -> void;
    static auto clear() -> void;

    static auto set_camera(std::shared_ptr<Camera> camera) -> void;

    static auto submit(const Drawable& drawable, const RenderStates& render_states) -> void;

    // TODO: these shouldn't be public
    static auto draw(const CubeShape& cube) -> void;
    static auto draw(const Mesh& mesh) -> void;
    static auto draw(const VertexArray& vertex_array) -> void;

    static auto render() -> void;

private:
    static inline std::shared_ptr<Camera> _camera = nullptr;
    static inline std::vector<DrawCommand> _draw_commands;

private:
    static auto log_gl_version() -> void;

    static auto bind_render_states(const RenderStates& render_states) -> void;
    static auto execute(const DrawCommand& draw_command) -> void;
};

} // namespace zth
