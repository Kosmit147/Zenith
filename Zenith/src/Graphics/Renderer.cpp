#include "Zenith/Graphics/Renderer.hpp"

#include <glad/glad.h>
#include <glm/gtc/matrix_access.hpp>

#include "Zenith/Core/Assert.hpp"
#include "Zenith/Graphics/Colors.hpp"
#include "Zenith/Graphics/Material.hpp"
#include "Zenith/Graphics/Materials.hpp"
#include "Zenith/Graphics/Mesh.hpp"
#include "Zenith/Graphics/Meshes.hpp"
#include "Zenith/Graphics/Shaders.hpp"
#include "Zenith/Graphics/Shapes/Shapes.hpp"
#include "Zenith/Logging/Logger.hpp"
#include "Zenith/Math/Matrix.hpp"
#include "Zenith/Platform/Event.hpp"
#include "Zenith/Platform/OpenGl/GlDebug.hpp"
#include "Zenith/Platform/OpenGl/Shader.hpp"
#include "Zenith/Platform/OpenGl/Texture.hpp"
#include "Zenith/Platform/OpenGl/VertexArray.hpp"

namespace zth {

namespace {

std::unique_ptr<Renderer> renderer;

} // namespace

auto Renderer::init() -> void
{
#ifdef _DEBUG
    enable_gl_debug();
#endif

    log_gl_version();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    set_clear_color(colors::transparent);

    renderer.reset(new Renderer);

    shaders::load_shaders();
    meshes::load_meshes();
    materials::init_materials();

    renderer->_instance_buffer.set_layout(VertexBufferLayout::from_vertex<InstanceBufferElement>());
    renderer->_instance_buffer.set_stride(sizeof(InstanceBufferElement));

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
    meshes::unload_meshes();
    shaders::unload_shaders();

    renderer.reset();

    ZTH_CORE_INFO("Renderer shut down.");
}

auto Renderer::set_clear_color(glm::vec4 color) -> void
{
    glClearColor(color.r, color.g, color.b, color.a);
}

auto Renderer::clear() -> void
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

auto Renderer::set_camera(std::shared_ptr<const PerspectiveCamera> camera) -> void
{
    renderer->_camera = std::move(camera);
}

auto Renderer::set_light(std::shared_ptr<const Light> light) -> void
{
    renderer->_light = std::move(light);
}

auto Renderer::draw(const CubeShape& cube, const Material& material) -> void
{
    draw(cube.mesh(), cube.transform(), material);
}

auto Renderer::draw(const SphereShape& sphere, const Material& material) -> void
{
    draw(sphere.mesh(), sphere.transform(), material);
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
    auto& tmp_va = renderer->_tmp_va;

    ZTH_ASSERT(batch.vertex_array->vertex_buffer() != nullptr);
    ZTH_ASSERT(batch.vertex_array->index_buffer() != nullptr);

    instance_data.clear();

    // TODO: send 4 vec3s as transform cols to the shader instead of rows???

    for (auto& transform_ptr : batch.transforms)
    {
        const auto& transform = *transform_ptr;
        auto normal_matrix = get_normal_matrix(transform);
        instance_data.emplace_back(glm::row(transform, 0), glm::row(transform, 1), glm::row(transform, 2),
                                   normal_matrix[0], normal_matrix[1], normal_matrix[2]);
    }

    instance_buffer.buffer_data(instance_data);

    // TODO: get rid of this
    tmp_va.bind_vertex_buffer(*batch.vertex_array->vertex_buffer());
    tmp_va.bind_index_buffer(*batch.vertex_array->index_buffer());
    tmp_va.bind_instance_buffer(instance_buffer);

    draw_instanced(tmp_va, *batch.material, instance_data.size());

    tmp_va.unbind_all_buffers();
}

auto Renderer::upload_camera_ubo() -> void
{
    ZTH_ASSERT(renderer->_camera != nullptr);
    const auto& camera = renderer->_camera;

    CameraUboData camera_ubo_data = {
        .view_projection = camera->view_projection(),
        .camera_position = camera->position(),
    };

    renderer->_camera_ubo.buffer_data(camera_ubo_data);
}

auto Renderer::upload_light_ubo() -> void
{
    ZTH_ASSERT(renderer->_light != nullptr);
    const auto& light = renderer->_light;

    LightUboData light_ubo_data = {
        .light_position = light->translation(),
        .light_color = light->color,
        .light_ambient = light->ambient,
        .light_diffuse = light->diffuse,
        .light_specular = light->specular,
    };

    renderer->_light_ubo.buffer_data(light_ubo_data);
}

auto Renderer::bind_material(const Material& material) -> void
{
    ZTH_ASSERT(material.shader != nullptr);
    material.shader->bind();
    upload_material_ubo(material);

    if (material.texture)
    {
        u32 tex_slot = 0;
        material.texture->bind(tex_slot);
        material.shader->set_unif("tex", static_cast<GLint>(tex_slot));
    }
}

auto Renderer::upload_material_ubo(const Material& material) -> void
{
    MaterialUboData material_ubo_data = {
        .albedo = material.albedo,
        .has_texture = material.texture ? true : false,
        .material_ambient = material.ambient,
        .material_diffuse = material.diffuse,
        .material_specular = material.specular,
        .shininess = material.shininess,
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
