#include "Zenith/Graphics/Renderer.hpp"

#include <glad/glad.h>

#include "Zenith/Core/Assert.hpp"
#include "Zenith/Graphics/Camera.hpp"
#include "Zenith/Graphics/Colors.hpp"
#include "Zenith/Graphics/Mesh.hpp"
#include "Zenith/Logging/Logger.hpp"
#include "Zenith/Platform/Event.hpp"
#include "Zenith/Platform/OpenGl/GlBuffer.hpp"
#include "Zenith/Platform/OpenGl/GlDebug.hpp"
#include "Zenith/Platform/OpenGl/Shader.hpp"
#include "Zenith/Platform/OpenGl/Texture2D.hpp"
#include "Zenith/Platform/OpenGl/VertexArray.hpp"

namespace zth {

auto Renderer::init() -> void
{
#ifdef _DEBUG
    enable_gl_debug();
#endif

    log_gl_version();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    set_clear_color(colors::transparent);

    ZTH_CORE_INFO("Renderer initialized.");
}

auto Renderer::on_window_event(const Event& event) -> void
{
    ZTH_ASSERT(event.category() == EventCategory::WindowEvent);

    if (event.type() == EventType::WindowResized)
    {
        auto [new_size] = event.window_resized_event();
        auto [width, height] = new_size;
        glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
    }
}

auto Renderer::shut_down() -> void
{
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

auto Renderer::draw(const Mesh& mesh, const RenderStates& render_states) -> void
{
    draw(mesh.vertex_array(), render_states);
}

auto Renderer::draw(const VertexArray& vertex_array, const RenderStates& render_states) -> void
{
    bind_render_states(render_states);
    vertex_array.bind();
    glDrawElements(GL_TRIANGLES, vertex_array.count(), vertex_array.index_type(), nullptr);
}

auto Renderer::draw(const VertexBuffer& vertex_buffer, const RenderStates& render_states) -> void
{
    bind_render_states(render_states);
    vertex_buffer.bind();
    glDrawArrays(GL_TRIANGLES, 0, vertex_buffer.size());
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

auto Renderer::bind_render_states(const RenderStates& render_states) -> void
{
    auto& [transform, shader, texture] = render_states;

    if (shader)
    {
        shader->bind();

        ZTH_ASSERT(_camera != nullptr);
        shader->set_unif("view_projection", _camera->view_projection());
    }
    else
    {
        // TODO: bind default shader?
        shader->unbind();
    }

    if (transform)
        shader->set_unif("transform", *transform);

    if (texture)
        texture->bind();
    else
        texture->unbind();

    if (shader && texture)
        shader->set_unif("tex", 0);
}

} // namespace zth
