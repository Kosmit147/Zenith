#pragma once

#include <glm/fwd.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "zenith/core/typedefs.hpp"
#include "zenith/ecs/fwd.hpp"
#include "zenith/gl/buffer.hpp"
#include "zenith/gl/fwd.hpp"
#include "zenith/renderer/fwd.hpp"
#include "zenith/renderer/light.hpp"
#include "zenith/renderer/shader_data.hpp"
#include "zenith/renderer/vertex.hpp"
#include "zenith/stl/string.hpp"
#include "zenith/stl/vector.hpp"
#include "zenith/system/fwd.hpp"
#include "zenith/util/result.hpp"

namespace zth {

struct DrawCommand
{
    const gl::VertexArray* vertex_array;
    const Material* material;
    const glm::mat4* transform;

    // Comparison operators are used to sort draw commands into batches.
    [[nodiscard]] auto operator==(const DrawCommand& other) const -> bool;
    [[nodiscard]] auto operator<(const DrawCommand& other) const -> bool;
    [[nodiscard]] auto operator>(const DrawCommand& other) const -> bool;
    [[nodiscard]] auto operator<=(const DrawCommand& other) const -> bool;
    [[nodiscard]] auto operator>=(const DrawCommand& other) const -> bool;
};

// Draw commands which use the same vertex array get merged into a render batch in order to utilize instanced rendering.
struct RenderBatch
{
    const gl::VertexArray* vertex_array;
    const Material* material;
    Vector<const glm::mat4*> transforms;
};

struct DirectionalLightRenderData
{
    glm::vec3 direction;
    LightProperties properties;
};

struct PointLightRenderData
{
    glm::vec3 position;
    LightProperties properties;
    LightAttenuation attenuation;
};

struct SpotLightRenderData
{
    glm::vec3 position;
    glm::vec3 direction;
    float inner_cutoff_cosine; // Cosine of the inner cone angle.
    float outer_cutoff_cosine; // Cosine of the outer cone angle.
    LightProperties properties;
    LightAttenuation attenuation;
};

struct AmbientLightRenderData
{
    glm::vec3 ambient;
};

// @test: Multiple directional lights.
// @test: Multiple spot lights.
// @test: Multiple ambient lights.

class Renderer
{
public:
    // @volatile: Keep in sync with constants declared in zth_defines.glsl.

    static constexpr u32 diffuse_map_slot = 0;
    static constexpr u32 specular_map_slot = 1;
    static constexpr u32 emission_map_slot = 2;

    static constexpr u32 camera_ubo_binding_point = 0;
    static constexpr u32 material_ubo_binding_point = 1;

    static constexpr u32 directional_lights_ssbo_binding_point = 0;
    static constexpr u32 point_lights_ssbo_binding_point = 1;
    static constexpr u32 spot_lights_ssbo_binding_point = 2;
    static constexpr u32 ambient_lights_ssbo_binding_point = 3;

    static constexpr usize initial_directional_lights_ssbo_size =
        sizeof(DirectionalLightsSsboData) + sizeof(DirectionalLightShaderData) * 3;
    static constexpr usize initial_point_lights_ssbo_size =
        sizeof(PointLightsSsboData) + sizeof(PointLightShaderData) * 10;
    static constexpr usize initial_spot_lights_ssbo_size =
        sizeof(SpotLightsSsboData) + sizeof(SpotLightShaderData) * 10;

    static constexpr usize initial_instance_buffer_size = sizeof(InstanceVertex) * 1000;

public:
    [[nodiscard]] static auto init() -> Result<void, String>;
    static auto on_window_event(const Event& event) -> void;
    static auto shut_down() -> void;

    static auto set_wireframe_mode(bool enabled) -> void;
    static auto toggle_wireframe_mode() -> void;

    static auto set_clear_color(glm::vec4 color) -> void;
    static auto clear() -> void;

    static auto begin_scene(const CameraComponent& camera, const TransformComponent& camera_transform) -> void;
    static auto end_scene() -> void;

    static auto submit_light(const LightComponent& light, const TransformComponent& light_transform) -> void;
    static auto submit_directional_light(const DirectionalLight& light, const TransformComponent& light_transform)
        -> void;
    static auto submit_point_light(const PointLight& light, const TransformComponent& light_transform) -> void;
    static auto submit_spot_light(const SpotLight& light, const TransformComponent& light_transform) -> void;
    static auto submit_ambient_light(const AmbientLight& light) -> void;

    // The submitted references must all be valid until the renderer finishes rendering the scene.
    static auto submit(const Mesh& mesh, const glm::mat4& transform, const Material& material) -> void;
    // The submitted references must all be valid until the renderer finishes rendering the scene.
    static auto submit(const gl::VertexArray& vertex_array, const glm::mat4& transform, const Material& material)
        -> void;

    [[nodiscard]] static auto current_camera_view() -> const glm::mat4&;
    [[nodiscard]] static auto current_camera_projection() -> const glm::mat4&;
    [[nodiscard]] static auto current_camera_view_projection() -> const glm::mat4&;
    [[nodiscard]] static auto wireframe_mode_enabled() { return _wireframe_mode_enabled; }

private:
    glm::mat4 _current_camera_view;
    glm::mat4 _current_camera_projection;
    glm::mat4 _current_camera_view_projection;

    Vector<DirectionalLightRenderData> _directional_lights;
    Vector<PointLightRenderData> _point_lights;
    Vector<SpotLightRenderData> _spot_lights;
    Vector<AmbientLightRenderData> _ambient_lights;

    gl::UniformBuffer _camera_ubo =
        gl::UniformBuffer::create_static_with_size(sizeof(CameraUboData), camera_ubo_binding_point);
    gl::UniformBuffer _material_ubo =
        gl::UniformBuffer::create_static_with_size(sizeof(MaterialUboData), material_ubo_binding_point);

    // @speed: It might be better to fit all lights into single buffer instead of creating a separate one for every kind
    // of light.

    gl::ShaderStorageBuffer _directional_lights_ssbo = gl::ShaderStorageBuffer::create_dynamic_with_size(
        initial_directional_lights_ssbo_size, directional_lights_ssbo_binding_point);
    gl::ShaderStorageBuffer _point_lights_ssbo = gl::ShaderStorageBuffer::create_dynamic_with_size(
        initial_point_lights_ssbo_size, point_lights_ssbo_binding_point);
    gl::ShaderStorageBuffer _spot_lights_ssbo = gl::ShaderStorageBuffer::create_dynamic_with_size(
        initial_spot_lights_ssbo_size, spot_lights_ssbo_binding_point);
    gl::ShaderStorageBuffer _ambient_lights_ssbo =
        gl::ShaderStorageBuffer::create_dynamic(ambient_lights_ssbo_binding_point);

    // Right now we're drawing everything using instanced rendering, even if the number of objects to draw is only 1 as
    // there doesn't appear to be any drawback to doing so. Crucially, this means that every vertex array needs to be
    // bound to the renderer's instance buffer before issuing the draw call.
    Vector<InstanceVertex> _instance_data;
    gl::InstanceBuffer _instance_buffer =
        gl::InstanceBuffer::create_dynamic_with_size(initial_instance_buffer_size, sizeof(InstanceVertex));

    Vector<DrawCommand> _draw_commands;
    Vector<RenderBatch> _batches;

    static inline bool _wireframe_mode_enabled = false;

private:
    explicit Renderer() = default;

    static auto render() -> void;

    static auto draw_indexed(const gl::VertexArray& vertex_array, const Material& material) -> void;
    static auto draw_instanced(const gl::VertexArray& vertex_array, const Material& material, u32 instances) -> void;

    static auto batch_draw_commands() -> void;
    static auto render_batch(const RenderBatch& batch) -> void;

    static auto bind_material(const Material& material) -> void;

    static auto upload_camera_data(glm::vec3 camera_position, const glm::mat4& view_projection) -> void;
    static auto upload_material_data(const Material& material) -> void;
    static auto upload_light_data() -> void;
    static auto upload_directional_lights_data() -> void;
    static auto upload_point_lights_data() -> void;
    static auto upload_spot_lights_data() -> void;
    static auto upload_ambient_lights_data() -> void;
};

} // namespace zth
