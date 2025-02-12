#pragma once

#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <memory>
#include <vector>

#include "zenith/core/typedefs.hpp"
#include "zenith/gl/buffer.hpp"
#include "zenith/gl/fwd.hpp"
#include "zenith/graphics/camera.hpp"
#include "zenith/graphics/fwd.hpp"
#include "zenith/graphics/light.hpp"
#include "zenith/graphics/shader_data.hpp"
#include "zenith/graphics/vertex.hpp"
#include "zenith/math/vector.hpp"
#include "zenith/system/fwd.hpp"

namespace zth {

struct DrawCommand
{
    const gl::VertexArray* vertex_array;
    const Material* material;
    const glm::mat4* transform;

    // comparison operators are used to sort draw commands into batches
    [[nodiscard]] auto operator==(const DrawCommand& other) const -> bool;
    [[nodiscard]] auto operator<(const DrawCommand& other) const -> bool;
    [[nodiscard]] auto operator>(const DrawCommand& other) const -> bool;
    [[nodiscard]] auto operator<=(const DrawCommand& other) const -> bool;
    [[nodiscard]] auto operator>=(const DrawCommand& other) const -> bool;
};

struct RenderBatch
{
    const gl::VertexArray* vertex_array;
    const Material* material;
    std::vector<const glm::mat4*> transforms;
};

// @test: multiple directional lights
// @test: multiple spot lights

class Renderer
{
public:
    // @volatile: keep these constants in sync with zth_defines.glsl

    static constexpr u32 diffuse_map_slot = 0;
    static constexpr u32 specular_map_slot = 1;
    static constexpr u32 emission_map_slot = 2;

    static constexpr u32 camera_ubo_binding_point = 0;
    static constexpr u32 material_ubo_binding_point = 1;

    static constexpr u32 directional_lights_ssbo_binding_point = 0;
    static constexpr u32 point_lights_ssbo_binding_point = 1;
    static constexpr u32 spot_lights_ssbo_binding_point = 2;

    static constexpr usize initial_directional_lights_ssbo_size =
        sizeof(DirectionalLightsSsboData) + sizeof(DirectionalLightData) * 3;
    static constexpr usize initial_point_lights_ssbo_size = sizeof(PointLightsSsboData) + sizeof(PointLightData) * 10;
    static constexpr usize initial_spot_lights_ssbo_size = sizeof(SpotLightsSsboData) + sizeof(SpotLightData) * 10;

public:
    static auto init() -> void;
    static auto on_window_event(const Event& event) -> void;
    static auto shut_down() -> void;

    static auto clear_scene_data() -> void;

    static auto set_wireframe_mode(bool enabled) -> void;
    static auto toggle_wireframe_mode() -> void;

    static auto set_clear_color(glm::vec4 color) -> void;
    static auto clear() -> void;

    static auto set_camera(std::shared_ptr<const PerspectiveCamera> camera) -> void;

    static auto add_directional_light(std::shared_ptr<const DirectionalLight> light) -> void;
    static auto remove_directional_light(usize index) -> void;
    static auto clear_directional_lights() -> void;
    static auto add_point_light(std::shared_ptr<const PointLight> light) -> void;
    static auto remove_point_light(usize index) -> void;
    static auto clear_point_lights() -> void;
    static auto add_spot_light(std::shared_ptr<const SpotLight> light) -> void;
    static auto remove_spot_light(usize index) -> void;
    static auto clear_spot_lights() -> void;

    static auto draw(const Shape3D& shape, const Material& material) -> void;
    static auto draw(const Mesh& mesh, const glm::mat4& transform, const Material& material) -> void;
    static auto draw(const gl::VertexArray& vertex_array, const glm::mat4& transform, const Material& material) -> void;

    static auto render() -> void;

    static auto draw_indexed(const gl::VertexArray& vertex_array, const Material& material) -> void;
    static auto draw_instanced(const gl::VertexArray& vertex_array, const Material& material, u32 instances) -> void;

    [[nodiscard]] static auto wireframe_mode_enabled() { return _wireframe_mode_enabled; }

    [[nodiscard]] static auto directional_light_count() -> usize;
    [[nodiscard]] static auto directional_lights() -> std::vector<std::shared_ptr<const DirectionalLight>>&;
    [[nodiscard]] static auto point_light_count() -> usize;
    [[nodiscard]] static auto point_lights() -> std::vector<std::shared_ptr<const PointLight>>&;
    [[nodiscard]] static auto spot_light_count() -> usize;
    [[nodiscard]] static auto spot_lights() -> std::vector<std::shared_ptr<const SpotLight>>&;

private:
    std::shared_ptr<const PerspectiveCamera> _camera =
        std::make_shared<PerspectiveCamera>(glm::vec3{ 0.0f }, math::world_forward, 1.0f);

    std::vector<std::shared_ptr<const DirectionalLight>> _directional_lights;
    std::vector<std::shared_ptr<const PointLight>> _point_lights;
    std::vector<std::shared_ptr<const SpotLight>> _spot_lights;

    gl::UniformBuffer _camera_ubo = gl::UniformBuffer::create_static(sizeof(CameraUboData), camera_ubo_binding_point);
    gl::UniformBuffer _material_ubo =
        gl::UniformBuffer::create_static(sizeof(MaterialUboData), material_ubo_binding_point);

    // @speed: it might be better to fit all lights into single buffer instead of creating a separate one for every kind
    // of light

    gl::ShaderStorageBuffer _directional_lights_ssbo = gl::ShaderStorageBuffer::create_dynamic(
        initial_directional_lights_ssbo_size, directional_lights_ssbo_binding_point);
    gl::ShaderStorageBuffer _point_lights_ssbo =
        gl::ShaderStorageBuffer::create_dynamic(initial_point_lights_ssbo_size, point_lights_ssbo_binding_point);
    gl::ShaderStorageBuffer _spot_lights_ssbo =
        gl::ShaderStorageBuffer::create_dynamic(initial_spot_lights_ssbo_size, spot_lights_ssbo_binding_point);

    std::vector<InstanceVertex> _instance_data;
    gl::InstanceBuffer _instance_buffer = gl::InstanceBuffer::create_dynamic();

    std::vector<DrawCommand> _draw_commands;
    std::vector<RenderBatch> _batches;

    static inline bool _wireframe_mode_enabled = false;

private:
    explicit Renderer() = default;

    static auto batch_draw_commands() -> void;
    static auto render_batch(const RenderBatch& batch) -> void;

    static auto bind_material(const Material& material) -> void;

    static auto upload_camera_data() -> void;
    static auto upload_material_data(const Material& material) -> void;
    static auto upload_light_data() -> void;
    static auto upload_directional_lights_data() -> void;
    static auto upload_point_lights_data() -> void;
    static auto upload_spot_lights_data() -> void;
};

} // namespace zth
