#include "zenith/graphics/renderer.hpp"

#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

#include "zenith/core/assert.hpp"
#include "zenith/graphics/colors.hpp"
#include "zenith/graphics/material.hpp"
#include "zenith/graphics/materials.hpp"
#include "zenith/graphics/mesh.hpp"
#include "zenith/graphics/meshes.hpp"
#include "zenith/graphics/shaders.hpp"
#include "zenith/graphics/shape.hpp"
#include "zenith/logging/logger.hpp"
#include "zenith/math/matrix.hpp"
#include "zenith/platform/event.hpp"
#include "zenith/platform/gl/gl_debug.hpp"
#include "zenith/platform/gl/shader.hpp"
#include "zenith/platform/gl/texture.hpp"
#include "zenith/platform/gl/vertex_array.hpp"

namespace zth {

namespace {

std::unique_ptr<Renderer> renderer;

} // namespace

auto Renderer::init() -> void
{
#ifndef ZTH_DIST_BUILD
    enable_gl_debug();
#endif

    log_gl_version();

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
    materials::unload_materials();
    meshes::unload_meshes();
    shaders::unload_shaders();

    renderer.reset();

    ZTH_CORE_INFO("Renderer shut down.");
}

auto Renderer::set_clear_color(glm::vec4 color) -> void
{
    glClearColor(color.r, color.g, color.b, color.a);
}

auto Renderer::set_wireframe_mode(bool enabled) -> void
{
    glPolygonMode(GL_FRONT_AND_BACK, enabled ? GL_LINE : GL_FILL);
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

auto Renderer::set_directional_light(std::shared_ptr<const DirectionalLight> directional_light) -> void
{
    renderer->_directional_light = std::move(directional_light);
}

auto Renderer::set_point_light(std::shared_ptr<const PointLight> point_light) -> void
{
    renderer->_point_light = std::move(point_light);
}

auto Renderer::set_spot_light(std::shared_ptr<const SpotLight> spot_light) -> void
{
    renderer->_spot_light = std::move(spot_light);
}

auto Renderer::draw(const Shape3D& shape, const Material& material) -> void
{
    draw(shape.mesh(), shape.transform(), material);
}

auto Renderer::draw(const Mesh& mesh, const glm::mat4& transform, const Material& material) -> void
{
    draw(mesh.vertex_array(), transform, material);
}

auto Renderer::draw(const VertexArray& vertex_array, const glm::mat4& transform, const Material& material) -> void
{
    renderer->_draw_commands.emplace_back(&vertex_array, &material, &transform);
}

auto Renderer::render() -> void
{
    upload_camera_ubo();
    upload_light_ubo();
    batch_draw_commands();

    for (const auto& batch : renderer->_batches)
        render_batch(batch);

    renderer->_draw_commands.clear();
    renderer->_batches.clear();
}

auto Renderer::draw_indexed(const VertexArray& vertex_array, const Material& material) -> void
{
    vertex_array.bind();
    bind_material(material);

    glDrawElements(GL_TRIANGLES, vertex_array.count(), vertex_array.index_type(), nullptr);
}

auto Renderer::draw_instanced(const VertexArray& vertex_array, const Material& material, usize instances) -> void
{
    vertex_array.bind();
    bind_material(material);

    glDrawElementsInstanced(GL_TRIANGLES, vertex_array.count(), vertex_array.index_type(), nullptr,
                            static_cast<GLsizei>(instances));
}

auto Renderer::batch_draw_commands() -> void
{
    auto& draw_commands = renderer->_draw_commands;
    auto& batches = renderer->_batches;

    std::ranges::sort(draw_commands);
    static std::vector<const glm::mat4*> transforms;
    transforms.clear();

    for (usize i = 0; i < draw_commands.size(); i++)
    {
        const VertexArray* current_vertex_array = draw_commands[i].vertex_array;
        const Material* current_material = draw_commands[i].material;
        transforms.push_back(draw_commands[i].transform);

        while (i + 1 < draw_commands.size() && draw_commands[i + 1].material == current_material)
        {
            transforms.push_back(draw_commands[i + 1].transform);
            i++;
        }

        RenderBatch batch = {
            .vertex_array = current_vertex_array,
            .material = current_material,
            .transforms = {},
        };

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
    draw_instanced(*batch.vertex_array, *batch.material, instance_data.size());
}

auto Renderer::upload_camera_ubo() -> void
{
    ZTH_ASSERT(renderer->_camera != nullptr);
    const auto& camera = renderer->_camera;

    CameraUboData camera_ubo_data = {
        .view_projection = camera->view_projection(),
        .camera_position = camera->translation(),
    };

    renderer->_camera_ubo.buffer_data(camera_ubo_data);
}

auto Renderer::upload_light_ubo() -> void
{
    LightUboData light_ubo_data{};

    if (renderer->_directional_light)
    {
        const auto& [direction, properties] = *renderer->_directional_light;
        light_ubo_data.has_directional_light = true;

        light_ubo_data.directional_light_direction = direction;
        light_ubo_data.directional_light_properties = {
            .color = properties.color,
            .ambient = properties.ambient,
            .diffuse = properties.diffuse,
            .specular = properties.specular,
        };
    }

    if (renderer->_point_light)
    {
        const auto& [position, properties, attenuation] = *renderer->_point_light;
        light_ubo_data.has_point_light = true;

        light_ubo_data.point_light_position = position;
        light_ubo_data.point_light_properties = {
            .color = properties.color,
            .ambient = properties.ambient,
            .diffuse = properties.diffuse,
            .specular = properties.specular,
        };
        light_ubo_data.point_light_attenuation = {
            .constant = attenuation.constant,
            .linear = attenuation.linear,
            .quadratic = attenuation.quadratic,
        };
    }

    if (renderer->_spot_light)
    {
        const auto& [position, direction, inner_cutoff, outer_cutoff, properties, attenuation] = *renderer->_spot_light;
        light_ubo_data.has_spot_light = true;

        light_ubo_data.spot_light_position = position;
        light_ubo_data.spot_light_direction = direction;
        light_ubo_data.spot_light_inner_cutoff = inner_cutoff;
        light_ubo_data.spot_light_outer_cutoff = outer_cutoff;
        light_ubo_data.spot_light_properties = {
            .color = properties.color,
            .ambient = properties.ambient,
            .diffuse = properties.diffuse,
            .specular = properties.specular,
        };
        light_ubo_data.spot_light_attenuation = {
            .constant = attenuation.constant,
            .linear = attenuation.linear,
            .quadratic = attenuation.quadratic,
        };
    }

    renderer->_light_ubo.buffer_data(light_ubo_data);
}

auto Renderer::bind_material(const Material& material) -> void
{
    ZTH_ASSERT(material.shader != nullptr);
    material.shader->bind();
    upload_material_ubo(material);

    if (material.diffuse_map)
    {
        material.diffuse_map->bind(diffuse_map_slot);
        material.shader->set_unif("diffuseMap", diffuse_map_slot);
    }

    if (material.specular_map)
    {
        material.specular_map->bind(specular_map_slot);
        material.shader->set_unif("specularMap", specular_map_slot);
    }

    if (material.emission_map)
    {
        material.emission_map->bind(emission_map_slot);
        material.shader->set_unif("emissionMap", emission_map_slot);
    }
}

auto Renderer::upload_material_ubo(const Material& material) -> void
{
    MaterialUboData material_ubo_data = {
        .material = {
            .albedo = material.albedo,
            .ambient = material.ambient,
            .diffuse = material.diffuse,
            .specular = material.specular,
            .shininess = material.shininess,
        },
        .has_diffuse_map = material.diffuse_map != nullptr,
        .has_specular_map = material.specular_map != nullptr,
        .has_emission_map = material.emission_map != nullptr,
    };

    renderer->_material_ubo.buffer_data(material_ubo_data);
}

auto Renderer::log_gl_version() -> void
{
    auto vendor_str = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    auto renderer_str = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    auto version_str = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    auto glsl_version_str = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));

    ZTH_CORE_INFO("[Renderer] OpenGL context info:\n"
                  "\tVendor: {}\n"
                  "\tRenderer: {}\n"
                  "\tVersion: {}\n"
                  "\tGLSL Version: {}",
                  vendor_str, renderer_str, version_str, glsl_version_str);
}

} // namespace zth
