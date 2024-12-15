#pragma once

#include <glm/fwd.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

#include <memory>
#include <vector>

#include "Zenith/Core/Typedefs.hpp"
#include "Zenith/Graphics/Camera.hpp"
#include "Zenith/Graphics/DrawCommand.hpp"
#include "Zenith/Graphics/fwd.hpp"
#include "Zenith/Platform/OpenGl/GlBuffer.hpp"
#include "Zenith/Platform/OpenGl/VertexArray.hpp"
#include "Zenith/Platform/OpenGl/fwd.hpp"

namespace zth {

class Event;

struct RenderBatch
{
    const VertexArray* vertex_array;
    const Material* material;
    std::vector<const glm::mat4*> transforms;
};

struct CameraMatrices
{
    glm::mat4 view_projection;
};

struct TransformBufferVertex
{
    glm::vec4 transform_col_0;
    glm::vec4 transform_col_1;
    glm::vec4 transform_col_2;
    glm::vec4 transform_col_3;
};

class Renderer
{
public:
    static auto init() -> void;
    static auto on_window_event(const Event& event) -> void;
    static auto shut_down() -> void;

    static auto set_clear_color(glm::vec4 color) -> void;
    static auto clear() -> void;

    static auto set_camera(std::shared_ptr<Camera> camera) -> void;

    static auto draw(const CubeShape& cube, const Material& material) -> void;
    static auto draw(const Mesh& mesh, const glm::mat4& transform, const Material& material) -> void;
    static auto draw(const VertexArray& vertex_array, const glm::mat4& transform, const Material& material) -> void;

    static auto render() -> void;

    static auto draw_indexed(const VertexArray& vertex_array, const Material& material) -> void;
    static auto draw_instanced(const VertexArray& vertex_array, const Material& material, usize instances) -> void;

private:
    std::shared_ptr<Camera> _camera = std::make_shared<PerspectiveCamera>(glm::vec3{ 1.0f }, glm::vec3{ 1.0f }, 1.0f);

    UniformBuffer _camera_matrices_buffer = UniformBuffer::create_static(sizeof(CameraMatrices));

    std::vector<TransformBufferVertex> _transforms;
    InstanceBuffer _transforms_buffer = InstanceBuffer::create_dynamic();

    VertexArray _tmp_va; // TODO: get rid of this

    std::vector<DrawCommand> _draw_commands;
    std::vector<RenderBatch> _batches;

private:
    explicit Renderer() = default;

    static auto batch_draw_commands() -> void;
    static auto render_batch(const RenderBatch& batch) -> void;

    static auto upload_camera_matrices() -> void;

    static auto log_gl_version() -> void;
};

} // namespace zth
