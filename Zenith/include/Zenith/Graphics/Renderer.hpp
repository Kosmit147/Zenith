#pragma once

#include <glad/glad.h>
#include <glm/fwd.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <memory>
#include <vector>

#include "Zenith/Core/Typedefs.hpp"
#include "Zenith/Graphics/Camera.hpp"
#include "Zenith/Graphics/DrawCommand.hpp"
#include "Zenith/Graphics/Light.hpp"
#include "Zenith/Graphics/ShaderDefines.h"
#include "Zenith/Graphics/Vertex.hpp"
#include "Zenith/Graphics/fwd.hpp"
#include "Zenith/Platform/OpenGl/GlBuffer.hpp"
#include "Zenith/Platform/OpenGl/fwd.hpp"

namespace zth {

class Event;

struct RenderBatch
{
    const VertexArray* vertex_array;
    const Material* material;
    std::vector<const glm::mat4*> transforms;
};

struct CameraUboData
{
    glm::mat4 view_projection;
    glm::vec3 camera_position;
};

struct LightUboData
{
    glm::vec3 light_position;
    GLubyte pad1[4]{};
    glm::vec3 light_color;
    GLubyte pad2[4]{};

    glm::vec3 light_ambient;
    GLubyte pad3[4]{};
    glm::vec3 light_diffuse;
    GLubyte pad4[4]{};
    glm::vec3 light_specular;
};

struct MaterialUboData
{
    glm::vec3 albedo;
    GLboolean has_texture;
    GLubyte pad1[3]{};

    glm::vec3 material_ambient;
    GLubyte pad2[4]{};
    glm::vec3 material_diffuse;
    GLubyte pad3[4]{};
    glm::vec3 material_specular;
    GLfloat shininess;
};

class Renderer
{
public:
    static auto init() -> void;
    static auto on_window_event(const Event& event) -> void;
    static auto shut_down() -> void;

    static auto set_clear_color(glm::vec4 color) -> void;
    static auto set_wireframe_mode(bool enabled) -> void;
    static auto clear() -> void;

    static auto set_camera(std::shared_ptr<const PerspectiveCamera> camera) -> void;
    static auto set_light(std::shared_ptr<const Light> light) -> void;

    static auto draw(const CubeShape& cube, const Material& material) -> void;
    static auto draw(const SphereShape& sphere, const Material& material) -> void;
    static auto draw(const Mesh& mesh, const glm::mat4& transform, const Material& material) -> void;
    static auto draw(const VertexArray& vertex_array, const glm::mat4& transform, const Material& material) -> void;

    static auto render() -> void;

    static auto draw_indexed(const VertexArray& vertex_array, const Material& material) -> void;
    static auto draw_instanced(const VertexArray& vertex_array, const Material& material, usize instances) -> void;

private:
    std::shared_ptr<const PerspectiveCamera> _camera =
        std::make_shared<PerspectiveCamera>(glm::vec3{ 1.0f }, glm::vec3{ 1.0f }, 1.0f);
    std::shared_ptr<const Light> _light = std::make_shared<PointLight>(glm::vec3{ 0.0f }, glm::vec3{ 1.0f });

    UniformBuffer _camera_ubo = UniformBuffer::create_static(sizeof(CameraUboData), ZTH_CAMERA_UBO_BINDING_INDEX);
    UniformBuffer _light_ubo = UniformBuffer::create_static(sizeof(LightUboData), ZTH_LIGHT_UBO_BINDING_INDEX);
    UniformBuffer _material_ubo = UniformBuffer::create_static(sizeof(MaterialUboData), ZTH_MATERIAL_UBO_BINDING_INDEX);

    std::vector<InstanceVertex> _instance_data;
    InstanceBuffer _instance_buffer = InstanceBuffer::create_dynamic();

    std::vector<DrawCommand> _draw_commands;
    std::vector<RenderBatch> _batches;

private:
    explicit Renderer() = default;

    static auto batch_draw_commands() -> void;
    static auto render_batch(const RenderBatch& batch) -> void;

    static auto upload_camera_ubo() -> void;
    static auto upload_light_ubo() -> void;

    static auto bind_material(const Material& material) -> void;
    static auto upload_material_ubo(const Material& material) -> void;

    static auto log_gl_version() -> void;
};

} // namespace zth
