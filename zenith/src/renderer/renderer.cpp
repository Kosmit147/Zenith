#include "zenith/renderer/renderer.hpp"

#include <glad/glad.h>
#include <glm/gtx/structured_bindings.hpp>

#include "zenith/core/assert.hpp"
#include "zenith/ecs/components.hpp"
#include "zenith/gl/shader.hpp"
#include "zenith/gl/texture.hpp"
#include "zenith/gl/util.hpp"
#include "zenith/gl/vertex_array.hpp"
#include "zenith/log/logger.hpp"
#include "zenith/math/matrix.hpp"
#include "zenith/memory/managed.hpp"
#include "zenith/renderer/colors.hpp"
#include "zenith/renderer/material.hpp"
#include "zenith/renderer/mesh.hpp"
#include "zenith/renderer/resources/buffers.hpp"
#include "zenith/renderer/resources/materials.hpp"
#include "zenith/renderer/resources/meshes.hpp"
#include "zenith/renderer/resources/shaders.hpp"
#include "zenith/renderer/resources/textures.hpp"
#include "zenith/system/event.hpp"

namespace zth {

namespace {

UniquePtr<Renderer> renderer;

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

// This constructor exists only for the purpose of allowing make_unique to construct an instance of the Renderer.
Renderer::Renderer(Passkey) : Renderer() {}

auto Renderer::init() -> Result<void, String>
{
    ZTH_INTERNAL_TRACE("Initializing renderer...");

    buffers::create();
    textures::create();

    renderer = make_unique<Renderer>(Passkey{});

    set_blending_enabled(true);
    set_depth_test_enabled(true);
    set_face_culling_enabled(true);
    set_multisampling_enabled(true);
    set_wireframe_mode_enabled(false);
    set_clear_color(colors::transparent);

    shaders::load();
    meshes::load();
    materials::load();

    ZTH_INTERNAL_TRACE("Renderer initialized.");
    return {};
}

auto Renderer::start_frame() -> void
{
    renderer->_draw_calls_last_frame = renderer->_draw_calls_this_frame;
    renderer->_draw_calls_this_frame = 0;
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
    ZTH_INTERNAL_TRACE("Shutting down renderer...");

    // Unload order should be the opposite of load order.
    materials::unload();
    meshes::unload();
    shaders::unload();

    renderer.free();

    textures::destroy();
    buffers::destroy();

    ZTH_INTERNAL_TRACE("Renderer shut down.");
}

auto Renderer::set_blending_enabled(bool enabled) -> void
{
    if (enabled)
    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
    }
    else
    {
        glDisable(GL_BLEND);
    }

    renderer->_blending_enabled = enabled;
}

auto Renderer::set_depth_test_enabled(bool enabled) -> void
{
    if (enabled)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);

    renderer->_depth_test_enabled = enabled;
}

auto Renderer::set_face_culling_enabled(bool enabled) -> void
{
    if (enabled)
    {
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }

    renderer->_face_culling_enabled = enabled;
}

auto Renderer::set_multisampling_enabled(bool enabled) -> void
{
    if (enabled)
        glEnable(GL_MULTISAMPLE);
    else
        glDisable(GL_MULTISAMPLE);

    renderer->_multisampling_enabled = enabled;
}

auto Renderer::set_wireframe_mode_enabled(bool enabled) -> void
{
    if (enabled)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    renderer->_wireframe_mode_enabled = enabled;
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
    auto view = camera.view(camera_transform);
    auto projection = camera.projection();
    auto view_projection = projection * view;

    renderer->_current_camera_position = camera_transform.translation();
    renderer->_current_camera_view = view;
    renderer->_current_camera_projection = projection;
    renderer->_current_camera_view_projection = view_projection;
}

auto Renderer::end_scene() -> void
{
    render();
    reset_renderer_state();
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

auto Renderer::current_camera_position() -> glm::vec3
{
    return renderer->_current_camera_position;
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

auto Renderer::blending_enabled() -> bool
{
    return renderer->_blending_enabled;
}

auto Renderer::depth_test_enabled() -> bool
{
    return renderer->_depth_test_enabled;
}

auto Renderer::face_culling_enabled() -> bool
{
    return renderer->_face_culling_enabled;
}

auto Renderer::multisampling_enabled() -> bool
{
    return renderer->_multisampling_enabled;
}

auto Renderer::wireframe_mode_enabled() -> bool
{
    return renderer->_wireframe_mode_enabled;
}

auto Renderer::draw_calls_last_frame() -> u32
{
    return renderer->_draw_calls_last_frame;
}

auto Renderer::instance_buffer() -> const gl::InstanceBuffer&
{
    return renderer->_instance_buffer;
}

auto Renderer::render() -> void
{
    clear();

    upload_camera_data(renderer->_current_camera_position, renderer->_current_camera_view_projection);
    upload_light_data();
    batch_draw_commands();

    for (const auto& batch : renderer->_batches)
        render_batch(batch);
}

auto Renderer::draw_indexed(const gl::VertexArray& vertex_array, const Material& material) -> void
{
    vertex_array.bind();
    bind_material(material);

    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vertex_array.count()),
                   gl::to_gl_enum(vertex_array.indexing_data_type()), nullptr);

    renderer->_draw_calls_this_frame++;
}

auto Renderer::draw_instanced(const gl::VertexArray& vertex_array, const Material& material, u32 instances) -> void
{
    vertex_array.bind();
    bind_material(material);

    glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(vertex_array.count()),
                            gl::to_gl_enum(vertex_array.indexing_data_type()), nullptr,
                            static_cast<GLsizei>(instances));

    renderer->_draw_calls_this_frame++;
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
    auto& instance_data = renderer->_temporary_instance_data;
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
    else
        textures::white()->bind(diffuse_map_slot);

    if (material.specular_map)
        material.specular_map->bind(specular_map_slot);
    else
        textures::white()->bind(specular_map_slot);

    if (material.emission_map)
        material.emission_map->bind(emission_map_slot);
    else
        textures::black()->bind(emission_map_slot);
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

auto Renderer::reset_renderer_state() -> void
{
    renderer->_draw_commands.clear();
    renderer->_batches.clear(); // Should clear the batches before the next frame because they use temporary storage.

    renderer->_directional_lights.clear();
    renderer->_point_lights.clear();
    renderer->_spot_lights.clear();
    renderer->_ambient_lights.clear();
}

} // namespace zth
