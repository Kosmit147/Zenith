#include "zenith/renderer/renderer.hpp"

#include <glad/glad.h>
#include <glm/gtx/structured_bindings.hpp>

#include "zenith/core/assert.hpp"
#include "zenith/ecs/components.hpp"
#include "zenith/gl/context.hpp"
#include "zenith/gl/shader.hpp"
#include "zenith/gl/texture.hpp"
#include "zenith/gl/vertex_array.hpp"
#include "zenith/log/logger.hpp"
#include "zenith/math/matrix.hpp"
#include "zenith/renderer/colors.hpp"
#include "zenith/renderer/material.hpp"
#include "zenith/renderer/materials.hpp"
#include "zenith/renderer/mesh.hpp"
#include "zenith/renderer/meshes.hpp"
#include "zenith/renderer/shaders.hpp"
#include "zenith/system/event.hpp"

namespace zth {

namespace {

std::unique_ptr<Renderer> renderer;

} // namespace

auto DrawCommand::operator==(const DrawCommand& other) const -> bool
{
    // Ignore transform.
    return vertex_array == other.vertex_array && material == other.material;
}

auto DrawCommand::operator<(const DrawCommand& other) const -> bool
{
    // Sort by vertex array first.
    if (vertex_array < other.vertex_array)
        return true;
    else if (vertex_array > other.vertex_array)
        return false;

    // Sort by material second.
    if (material < other.material)
        return true;

    return false;
}

auto DrawCommand::operator>(const DrawCommand& other) const -> bool
{
    // Sort by vertex array first.
    if (vertex_array > other.vertex_array)
        return true;
    else if (vertex_array < other.vertex_array)
        return false;

    // Sort by material second.
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

    // We need to pass in the instance buffer as every vertex array needs to be bound to it because we're always doing
    // instanced rendering. As the vertex arrays in meshes are constant, they need to be initialized with the instance
    // buffer in this call.
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

auto Renderer::begin_scene(const CameraComponent& camera, const TransformComponent& camera_transform) -> void
{
    renderer->_draw_commands.clear();
    renderer->_batches.clear();

    renderer->_directional_lights.clear();
    renderer->_point_lights.clear();
    renderer->_spot_lights.clear();
    renderer->_ambient_lights.clear();

    clear();

    auto view = camera.view(camera_transform);
    auto projection = camera.projection();
    auto view_projection = projection * view;

    renderer->_current_camera_view = view;
    renderer->_current_camera_projection = projection;
    renderer->_current_camera_view_projection = view_projection;
    upload_camera_data(camera_transform.translation(), view_projection);
}

auto Renderer::end_scene() -> void
{
    render();
}

auto Renderer::submit_light(const LightComponent& light, const TransformComponent& light_transform) -> void
{
    switch (light.type())
    {
        using enum LightType;
    case Directional:
        submit_directional_light(light.directional_light(), light_transform);
        break;
    case Point:
        submit_point_light(light.point_light(), light_transform);
        break;
    case Spot:
        submit_spot_light(light.spot_light(), light_transform);
        break;
    case Ambient:
        submit_ambient_light(light.ambient_light());
        break;
    }
}

auto Renderer::submit_directional_light(const DirectionalLight& light, const TransformComponent& light_transform)
    -> void
{
    renderer->_directional_lights.push_back(DirectionalLightRenderData{
        .direction = light_transform.direction(),
        .properties = light.properties,
    });
}

auto Renderer::submit_point_light(const PointLight& light, const TransformComponent& light_transform) -> void
{
    renderer->_point_lights.push_back(PointLightRenderData{
        .position = light_transform.translation(),
        .properties = light.properties,
        .attenuation = light.attenuation,
    });
}

auto Renderer::submit_spot_light(const SpotLight& light, const TransformComponent& light_transform) -> void
{
    renderer->_spot_lights.push_back(SpotLightRenderData{
        .position = light_transform.translation(),
        .direction = light_transform.direction(),
        .inner_cutoff_cosine = light.inner_cutoff_cosine,
        .outer_cutoff_cosine = light.outer_cutoff_cosine,
        .properties = light.properties,
        .attenuation = light.attenuation,
    });
}

auto Renderer::submit_ambient_light(const AmbientLight& light) -> void
{
    renderer->_ambient_lights.push_back(AmbientLightRenderData{
        .ambient = light.ambient,
    });
}

auto Renderer::submit(const Mesh& mesh, const glm::mat4& transform, const Material& material) -> void
{
    submit(mesh.vertex_array(), transform, material);
}

auto Renderer::submit(const gl::VertexArray& vertex_array, const glm::mat4& transform, const Material& material) -> void
{
    renderer->_draw_commands.emplace_back(&vertex_array, &material, &transform);
}

auto Renderer::current_camera_view() -> const glm::mat4&
{
    return renderer->_current_camera_view;
}

auto Renderer::current_camera_projection() -> const glm::mat4&
{
    return renderer->_current_camera_projection;
}

auto Renderer::current_camera_view_projection() -> const glm::mat4&
{
    return renderer->_current_camera_view_projection;
}

auto Renderer::render() -> void
{
    upload_light_data();
    batch_draw_commands();

    for (const auto& batch : renderer->_batches)
        render_batch(batch);
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

auto Renderer::batch_draw_commands() -> void
{
    // @speed: We should look into how we could batch draw commands more efficiently. It would probably also be more
    // optimal to copy the transforms rather than storing pointers to them for better cache efficiency.

    auto& draw_commands = renderer->_draw_commands;
    auto& batches = renderer->_batches;

    std::ranges::sort(draw_commands);

    // @cleanup: This shouldn't be static.
    static Vector<const glm::mat4*> transforms;
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

        // @speed: We could probably do something better here than copying all the pointers.
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

auto Renderer::upload_camera_data(glm::vec3 camera_position, const glm::mat4& view_projection) -> void
{
    CameraUboData camera_ubo_data = {
        .view_projection = view_projection,
        .camera_position = camera_position,
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
    // @speed: Maybe we should collect all the data first and then buffer it all at once in these functions?
    // Also: we probably shouldn't buffer all this data every frame, but only when it changes.

    upload_directional_lights_data();
    upload_point_lights_data();
    upload_spot_lights_data();
    upload_ambient_lights_data();
}

auto Renderer::upload_directional_lights_data() -> void
{
    DirectionalLightsSsboData directional_lights_ssbo_data = {
        .count = static_cast<GLuint>(renderer->_directional_lights.size()),
    };

    u32 offset = 0;
    offset += renderer->_directional_lights_ssbo.buffer_data(directional_lights_ssbo_data);

    for (const auto& [direction, properties] : renderer->_directional_lights)
    {
        DirectionalLightShaderData data = {
            .direction = direction,
            .properties = {
                .color = properties.color,
                .ambient = properties.ambient,
                .diffuse = properties.diffuse,
                .specular = properties.specular,
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

    for (const auto& [position, properties, attenuation] : renderer->_point_lights)
    {
        PointLightShaderData data = {
            .position = position,
            .properties = {
                .color = properties.color,
                .ambient = properties.ambient,
                .diffuse = properties.diffuse,
                .specular = properties.specular,
            },
            .attenuation = {
                .constant = attenuation.constant,
                .linear = attenuation.linear,
                .quadratic = attenuation.quadratic,
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

    for (const auto& [position, direction, inner_cutoff_cosine, outer_cutoff_cosine, properties, attenuation] :
         renderer->_spot_lights)
    {
        SpotLightShaderData data = {
            .position = position,
            .direction = direction,
            .inner_cutoff_cosine = inner_cutoff_cosine,
            .outer_cutoff_cosine = outer_cutoff_cosine,
            .properties = {
                .color = properties.color,
                .ambient = properties.ambient,
                .diffuse = properties.diffuse,
                .specular = properties.specular,
            },
            .attenuation = {
                .constant = attenuation.constant,
                .linear = attenuation.linear,
                .quadratic = attenuation.quadratic,
            },
        };

        offset += renderer->_spot_lights_ssbo.buffer_data(data, offset);
    }
}

auto Renderer::upload_ambient_lights_data() -> void
{
    AmbientLightsSsboData ambient_lights_ssbo_data = {
        .count = static_cast<GLuint>(renderer->_ambient_lights.size()),
    };

    u32 offset = 0;
    offset += renderer->_ambient_lights_ssbo.buffer_data(ambient_lights_ssbo_data);

    for (const auto& [ambient] : renderer->_ambient_lights)
    {
        AmbientLightShaderData data = {
            .ambient = ambient,
        };

        offset += renderer->_ambient_lights_ssbo.buffer_data(data, offset);
    }
}

} // namespace zth
