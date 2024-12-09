#pragma once

#include <glm/fwd.hpp>
#include <glm/vec4.hpp>

#include <memory>
#include <optional>
#include <vector>

#include "Zenith/Graphics/Camera.hpp"
#include "Zenith/Graphics/fwd.hpp"
#include "Zenith/Platform/OpenGl/GlBuffer.hpp"
#include "Zenith/Platform/OpenGl/fwd.hpp"

namespace zth {

class Event;

struct DrawCommand
{
    const Drawable* drawable;
    const glm::mat4* transform;
    const Material* material;
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

    static auto submit(const Shape3D& shape, const Material& material) -> void;
    static auto submit(const Drawable& drawable, const glm::mat4& transform, const Material& material) -> void;

    // TODO: these shouldn't be public
    static auto draw(const CubeShape& cube) -> void;
    static auto draw(const Mesh& mesh) -> void;
    static auto draw(const VertexArray& vertex_array) -> void;

    static auto render() -> void;

private:
    static inline std::shared_ptr<Camera> _camera =
        std::make_shared<PerspectiveCamera>(glm::vec3(1.0f), glm::vec3(1.0f), 1.0f);
    static inline std::vector<DrawCommand> _draw_commands;
    static inline std::optional<UniformBuffer> _camera_matrices_buffer;

private:
    static auto log_gl_version() -> void;

    static auto execute(const DrawCommand& draw_command) -> void;

    static auto upload_camera_matrices() -> void;
};

} // namespace zth
