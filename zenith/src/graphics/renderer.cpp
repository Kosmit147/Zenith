#include "zenith/graphics/renderer.hpp"

#include <glad/glad.h>
#include <glm/gtx/structured_bindings.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

#include "zenith/core/assert.hpp"
#include "zenith/gl/context.hpp"
#include "zenith/gl/shader.hpp"
#include "zenith/gl/texture.hpp"
#include "zenith/gl/vertex_array.hpp"
#include "zenith/graphics/colors.hpp"
#include "zenith/graphics/material.hpp"
#include "zenith/graphics/materials.hpp"
#include "zenith/graphics/mesh.hpp"
#include "zenith/graphics/meshes.hpp"
#include "zenith/graphics/shaders.hpp"
#include "zenith/graphics/shape.hpp"
#include "zenith/log/logger.hpp"
#include "zenith/math/matrix.hpp"
#include "zenith/system/event.hpp"

namespace zth {

namespace {

std::unique_ptr<Renderer> renderer;

} // namespace

auto DrawCommand::operator==(const DrawCommand& other) const -> bool
{
    // ignore transform
    return vertex_array == other.vertex_array && material == other.material;
}

auto DrawCommand::operator<(const DrawCommand& other) const -> bool
{
    // sort by vertex array first
    if (vertex_array < other.vertex_array)
        return true;
    else if (vertex_array > other.vertex_array)
        return false;

    // sort by material second
    if (material < other.material)
        return true;

    return false;
}

auto DrawCommand::operator>(const DrawCommand& other) const -> bool
{
    // sort by vertex array first
    if (vertex_array > other.vertex_array)
        return true;
    else if (vertex_array < other.vertex_array)
        return false;

    // sort by material second
    if (material > other.material)
        return true;

    return false;
}

auto DrawCommand::operator<=(const DrawCommand& other) const -> bool
{
    return *this < other || *this == other;
}

auto DrawCommand::operator>=(const DrawCommand& other) const -> bool
{
    return *this > other || *this == other;
}

auto Renderer::init() -> void
{
    ZTH_CORE_INFO("Initializing renderer...");

#if !defined(ZTH_DIST_BUILD)
    gl::set_debug_context();
#endif

    gl::log_context_info();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    renderer.reset(new Renderer);
    renderer->_instance_buffer.set_stride(sizeof(InstanceVertex));

    set_clear_color(colors::transparent);

    shaders::load_shaders();
    meshes::load_meshes(renderer->_instance_buffer);
    materials::load_materials();

    ZTH_CORE_INFO("Renderer initialized.");
}

auto Renderer::on_window_event(const Event& event) -> void
{
    ZTH_ASSERT(event.category() == EventCategory::WindowEvent);

    if (event.type() == EventType::WindowResized)
    {
        auto [new_size] = event.window_resized_event();
        glViewport(0, 0, static_cast<GLsizei>(new_size.x), static_cast<GLsizei>(new_size.y));
    }
}

auto Renderer::shut_down() -> void
{
    ZTH_CORE_INFO("Shutting down renderer...");

    materials::unload_materials();
    meshes::unload_meshes();
    shaders::unload_shaders();

    renderer.reset();

    ZTH_CORE_INFO("Renderer shut down.");
}

// @temporary: this function should probably be deleted once we have entity component system
auto Renderer::clear_scene_data() -> void
{
    clear_directional_lights();
    clear_point_lights();
    clear_spot_lights();
}

auto Renderer::set_wireframe_mode(bool enabled) -> void
{
    _wireframe_mode_enabled = enabled;
    glPolygonMode(GL_FRONT_AND_BACK, _wireframe_mode_enabled ? GL_LINE : GL_FILL);
}

auto Renderer::toggle_wireframe_mode() -> void
{
    set_wireframe_mode(!_wireframe_mode_enabled);
}

auto Renderer::set_clear_color(glm::vec4 color) -> void
{
    auto [r, g, b, a] = color;
    glClearColor(r, g, b, a);
}

auto Renderer::clear() -> void
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

auto Renderer::set_camera(std::shared_ptr<const PerspectiveCamera> camera) -> void
{
    ZTH_ASSERT(camera != nullptr);
    renderer->_camera = std::move(camera);
}

auto Renderer::add_directional_light(std::shared_ptr<const DirectionalLight> light) -> void
{
    renderer->_directional_lights.push_back(std::move(light));
}

auto Renderer::remove_directional_light(usize index) -> void
{
    auto& lights = renderer->_directional_lights;
    ZTH_ASSERT(index < lights.size());
    lights.erase(lights.begin() + static_cast<isize>(index));
}

auto Renderer::clear_directional_lights() -> void
{
    renderer->_directional_lights.clear();
}

auto Renderer::add_point_light(std::shared_ptr<const PointLight> light) -> void
{
    renderer->_point_lights.push_back(std::move(light));
}

auto Renderer::remove_point_light(usize index) -> void
{
    auto& lights = renderer->_point_lights;
    ZTH_ASSERT(index < lights.size());
    lights.erase(lights.begin() + static_cast<isize>(index));
}

auto Renderer::clear_point_lights() -> void
{
    renderer->_point_lights.clear();
}

auto Renderer::add_spot_light(std::shared_ptr<const SpotLight> light) -> void
{
    renderer->_spot_lights.push_back(std::move(light));
}

auto Renderer::remove_spot_light(usize index) -> void
{
    auto& lights = renderer->_spot_lights;
    ZTH_ASSERT(index < lights.size());
    lights.erase(lights.begin() + static_cast<isize>(index));
}

auto Renderer::clear_spot_lights() -> void
{
    renderer->_spot_lights.clear();
}

auto Renderer::draw(const Shape3D& shape, const Material& material) -> void
{
    draw(shape.mesh(), shape.transform(), material);
}

auto Renderer::draw(const Mesh& mesh, const glm::mat4& transform, const Material& material) -> void
{
    draw(mesh.vertex_array(), transform, material);
}

auto Renderer::draw(const gl::VertexArray& vertex_array, const glm::mat4& transform, const Material& material) -> void
{
    renderer->_draw_commands.emplace_back(&vertex_array, &material, &transform);
}

auto Renderer::render() -> void
{
    upload_camera_data();
    upload_light_data();
    batch_draw_commands();

    for (const auto& batch : renderer->_batches)
        render_batch(batch);

    renderer->_draw_commands.clear();
    renderer->_batches.clear();
}

auto Renderer::draw_indexed(const gl::VertexArray& vertex_array, const Material& material) -> void
{
    vertex_array.bind();
    bind_material(material);

    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vertex_array.count()), vertex_array.index_type(), nullptr);
}

auto Renderer::draw_instanced(const gl::VertexArray& vertex_array, const Material& material, u32 instances) -> void
{
    vertex_array.bind();
    bind_material(material);

    glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(vertex_array.count()), vertex_array.index_type(),
                            nullptr, static_cast<GLsizei>(instances));
}

auto Renderer::directional_light_count() -> usize
{
    return renderer->_directional_lights.size();
}

auto Renderer::directional_lights() -> std::vector<std::shared_ptr<const DirectionalLight>>&
{
    return renderer->_directional_lights;
}

auto Renderer::point_light_count() -> usize
{
    return renderer->_point_lights.size();
}

auto Renderer::point_lights() -> std::vector<std::shared_ptr<const PointLight>>&
{
    return renderer->_point_lights;
}

auto Renderer::spot_light_count() -> usize
{
    return renderer->_spot_lights.size();
}

auto Renderer::spot_lights() -> std::vector<std::shared_ptr<const SpotLight>>&
{
    return renderer->_spot_lights;
}

auto Renderer::batch_draw_commands() -> void
{
    // @speed: we could batch draw commands more efficiently
    // it would probably also be more optimal to copy the transforms rather than storing pointers to them for better
    // cache efficiency

    auto& draw_commands = renderer->_draw_commands;
    auto& batches = renderer->_batches;

    std::ranges::sort(draw_commands);

    // @cleanup: this shouldn't be static
    static std::vector<const glm::mat4*> transforms;
    transforms.clear();

    for (usize i = 0; i < draw_commands.size(); i++)
    {
        const auto* current_vertex_array = draw_commands[i].vertex_array;
        const auto* current_material = draw_commands[i].material;
        transforms.push_back(draw_commands[i].transform);

        while (i + 1 < draw_commands.size() && draw_commands[i + 1].material == current_material
               && draw_commands[i + 1].vertex_array == current_vertex_array)
        {
            transforms.push_back(draw_commands[i + 1].transform);
            i++;
        }

        RenderBatch batch = {
            .vertex_array = current_vertex_array,
            .material = current_material,
            .transforms = {},
        };

        // @speed: we could probably do something better here than copying all the pointers
        std::ranges::move(transforms, std::back_inserter(batch.transforms));
        batches.push_back(std::move(batch));
        transforms.clear();
    }
}

auto Renderer::render_batch(const RenderBatch& batch) -> void
{
    auto& instance_data = renderer->_instance_data;
    auto& instance_buffer = renderer->_instance_buffer;

    ZTH_ASSERT(batch.vertex_array->vertex_buffer() != nullptr);
    ZTH_ASSERT(batch.vertex_array->index_buffer() != nullptr);

    instance_data.clear();

    for (auto& transform_ptr : batch.transforms)
    {
        const auto& transform = *transform_ptr;
        auto normal_matrix = math::get_normal_matrix(transform);
        instance_data.emplace_back(transform[0], transform[1], transform[2], transform[3], normal_matrix);
    }

    instance_buffer.buffer_data(instance_data);
    draw_instanced(*batch.vertex_array, *batch.material, static_cast<u32>(instance_data.size()));
}

auto Renderer::bind_material(const Material& material) -> void
{
    ZTH_ASSERT(material.shader != nullptr);
    material.shader->bind();
    upload_material_data(material);

    if (material.diffuse_map)
        material.diffuse_map->bind(diffuse_map_slot);

    if (material.specular_map)
        material.specular_map->bind(specular_map_slot);

    if (material.emission_map)
        material.emission_map->bind(emission_map_slot);
}

// @cleanup: send data to buffers in a cleaner, less error-prone way

auto Renderer::upload_camera_data() -> void
{
    ZTH_ASSERT(renderer->_camera != nullptr);
    const auto& camera = renderer->_camera;

    CameraUboData camera_ubo_data = {
        .view_projection = camera->view_projection(),
        .camera_position = camera->translation(),
    };

    renderer->_camera_ubo.buffer_data(camera_ubo_data);
}

auto Renderer::upload_material_data(const Material& material) -> void
{
    MaterialUboData material_ubo_data = {
        .albedo = material.albedo,
        .ambient = material.ambient,
        .diffuse = material.diffuse,
        .specular = material.specular,
        .shininess = material.shininess,
        .has_diffuse_map = material.diffuse_map != nullptr,
        .has_specular_map = material.specular_map != nullptr,
        .has_emission_map = material.emission_map != nullptr,
    };

    renderer->_material_ubo.buffer_data(material_ubo_data);
}

auto Renderer::upload_light_data() -> void
{
    // @speed: maybe we should collect all the data first and then buffer it all at once in these functions?
    // @speed: we probably shouldn't buffer all this data every frame, but only when it changes

    upload_directional_lights_data();
    upload_point_lights_data();
    upload_spot_lights_data();
}

auto Renderer::upload_directional_lights_data() -> void
{
    DirectionalLightsSsboData directional_lights_ssbo_data = {
        .count = static_cast<GLuint>(renderer->_directional_lights.size()),
    };

    u32 offset = 0;
    offset += renderer->_directional_lights_ssbo.buffer_data(directional_lights_ssbo_data);

    for (const auto& light : renderer->_directional_lights)
    {
        DirectionalLightData data = {
            .direction = light->direction,
            .properties = {
                .color = light->properties.color,
                .ambient = light->properties.ambient,
                .diffuse = light->properties.diffuse,
                .specular = light->properties.specular,
            },
        };

        offset += renderer->_directional_lights_ssbo.buffer_data(data, offset);
    }
}

auto Renderer::upload_point_lights_data() -> void
{
    PointLightsSsboData point_lights_ssbo_data = {
        .count = static_cast<GLuint>(renderer->_point_lights.size()),
    };

    u32 offset = 0;
    offset += renderer->_point_lights_ssbo.buffer_data(point_lights_ssbo_data);

    for (const auto& light : renderer->_point_lights)
    {
        PointLightData data = {
            .position = light->position,
            .properties = {
                .color = light->properties.color,
                .ambient = light->properties.ambient,
                .diffuse = light->properties.diffuse,
                .specular = light->properties.specular,
            },
            .attenuation = {
                .constant = light->attenuation.constant,
                .linear = light->attenuation.linear,
                .quadratic = light->attenuation.quadratic,
            },
        };

        offset += renderer->_point_lights_ssbo.buffer_data(data, offset);
    }
}

auto Renderer::upload_spot_lights_data() -> void
{
    SpotLightsSsboData spot_lights_ssbo_data = {
        .count = static_cast<GLuint>(renderer->_spot_lights.size()),
    };

    u32 offset = 0;
    offset += renderer->_spot_lights_ssbo.buffer_data(spot_lights_ssbo_data);

    for (const auto& light : renderer->_spot_lights)
    {
        SpotLightData data = {
            .position = light->position,
            .direction = light->direction,
            .inner_cutoff = light->inner_cutoff,
            .outer_cutoff = light->outer_cutoff,
            .properties = {
                .color = light->properties.color,
                .ambient = light->properties.ambient,
                .diffuse = light->properties.diffuse,
                .specular = light->properties.specular,
            },
            .attenuation = {
                .constant = light->attenuation.constant,
                .linear = light->attenuation.linear,
                .quadratic = light->attenuation.quadratic,
            },
        };

        offset += renderer->_spot_lights_ssbo.buffer_data(data, offset);
    }
}

} // namespace zth
