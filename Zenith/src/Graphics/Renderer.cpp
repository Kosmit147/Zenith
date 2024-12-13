#include "Zenith/Graphics/Renderer.hpp"

#include <glad/glad.h>
#include <glm/mat4x4.hpp>

#include "Zenith/Core/Assert.hpp"
#include "Zenith/Graphics/Colors.hpp"
#include "Zenith/Graphics/Material.hpp"
#include "Zenith/Graphics/Mesh.hpp"
#include "Zenith/Graphics/Meshes.hpp"
#include "Zenith/Graphics/ShaderDefines.h"
#include "Zenith/Graphics/Shaders.hpp"
#include "Zenith/Graphics/Shapes/Shapes.hpp"
#include "Zenith/Logging/Logger.hpp"
#include "Zenith/Platform/Event.hpp"
#include "Zenith/Platform/OpenGl/GlDebug.hpp"
#include "Zenith/Platform/OpenGl/Shader.hpp"
#include "Zenith/Platform/OpenGl/VertexArray.hpp"

namespace zth {

struct CameraMatrices
{
    glm::mat4 view_projection;
};

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

    shaders::load_shaders();
    meshes::load_meshes();

    _camera_matrices_buffer.emplace(static_cast<GLsizei>(sizeof(CameraMatrices)));
    _camera_matrices_buffer->set_binding_index(ZTH_CAMERA_MATRICES_UBO_BINDING_INDEX);

    // TODO: fix hardcoded transforms buffer size
    _transforms_buffer.emplace(static_cast<GLsizei>(sizeof(glm::mat4) * 32000));
    _transforms_buffer->set_layout(VertexLayout::from_vertex<TransformBufferVertex>());
    _transforms_buffer->set_stride(sizeof(TransformBufferVertex));

    _tmp_va.emplace();

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

    _tmp_va.reset();
    _transforms_buffer.reset();
    _camera_matrices_buffer.reset();

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

auto Renderer::set_camera(std::shared_ptr<Camera> camera) -> void
{
    _camera = std::move(camera);
}

auto Renderer::draw(const CubeShape& cube, const Material& material) -> void
{
    draw(cube.mesh(), cube.transform(), material);
}

auto Renderer::draw(const Mesh& mesh, const glm::mat4& transform, const Material& material) -> void
{
    draw(mesh.vertex_array(), transform, material);
}

auto Renderer::draw(const VertexArray& vertex_array, const glm::mat4& transform, const Material& material) -> void
{
    _draw_commands.emplace_back(&vertex_array, &material, &transform);
}

auto Renderer::render() -> void
{
    upload_camera_matrices();
    batch_draw_commands();

    for (const auto& batch : _batches)
        render_batch(batch);

    _draw_commands.clear();
    _batches.clear();
}

auto Renderer::draw_indexed(const VertexArray& vertex_array, const Material& material) -> void
{
    vertex_array.bind();
    material.bind();
    glDrawElements(GL_TRIANGLES, vertex_array.count(), vertex_array.index_type(), nullptr);
}

auto Renderer::draw_instanced(const VertexArray& vertex_array, const Material& material, usize instances) -> void
{
    vertex_array.bind();
    material.bind();
    glDrawElementsInstanced(GL_TRIANGLES, vertex_array.count(), vertex_array.index_type(), nullptr,
                            static_cast<GLsizei>(instances));
}

auto Renderer::batch_draw_commands() -> void
{
    std::ranges::sort(_draw_commands);
    std::vector<const glm::mat4*> transforms;
    transforms.reserve(100);

    for (usize i = 0; i < _draw_commands.size(); i++)
    {
        const VertexArray* current_vertex_array = _draw_commands[i].vertex_array;
        const Material* current_material = _draw_commands[i].material;

        while (i < _draw_commands.size() && _draw_commands[i].material == current_material)
        {
            transforms.push_back(_draw_commands[i].transform);
            i++;
        }

        RenderBatch batch = {
            .vertex_array = current_vertex_array,
            .material = current_material,
            .transforms = {},
        };

        std::ranges::move(transforms, std::back_inserter(batch.transforms));
        _batches.push_back(std::move(batch));
        transforms.clear();
    }
}

auto Renderer::render_batch(const RenderBatch& batch) -> void
{
    ZTH_ASSERT(batch.vertex_array->vertex_buffer() != nullptr);
    ZTH_ASSERT(batch.vertex_array->index_buffer() != nullptr);

    _transforms.clear();

    for (auto& transform_ptr : batch.transforms)
    {
        auto& transform = *transform_ptr;
        _transforms.emplace_back(transform[0], transform[1], transform[2], transform[3]);
    }

    _transforms_buffer->buffer_sub_data(_transforms);

    _tmp_va->bind_vertex_buffer(*batch.vertex_array->vertex_buffer());
    _tmp_va->bind_index_buffer(*batch.vertex_array->index_buffer());
    _tmp_va->bind_instance_buffer(*_transforms_buffer);

    draw_instanced(*_tmp_va, *batch.material, _transforms.size());

    _tmp_va->unbind_all_buffers();
}

auto Renderer::upload_camera_matrices() -> void
{
    ZTH_ASSERT(_camera != nullptr);

    CameraMatrices camera_matrices = {
        .view_projection = _camera->view_projection(),
    };

    _camera_matrices_buffer->buffer_sub_data(camera_matrices);
}

auto Renderer::log_gl_version() -> void
{
    auto vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    auto renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    auto version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    auto glsl_version = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));

    ZTH_CORE_INFO("[Renderer] OpenGL context info:\n"
                  "\tVendor: {}\n"
                  "\tRenderer: {}\n"
                  "\tVersion: {}\n"
                  "\tGLSL Version: {}",
                  vendor, renderer, version, glsl_version);
}

} // namespace zth
