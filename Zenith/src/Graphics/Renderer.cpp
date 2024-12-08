#include "Zenith/Graphics/Renderer.hpp"

#include <glad/glad.h>

#include "Zenith/Core/Assert.hpp"
#include "Zenith/Graphics/Camera.hpp"
#include "Zenith/Graphics/Colors.hpp"
#include "Zenith/Graphics/Drawable.hpp"
#include "Zenith/Graphics/Material.hpp"
#include "Zenith/Graphics/Mesh.hpp"
#include "Zenith/Graphics/Meshes.hpp"
#include "Zenith/Graphics/Shaders.hpp"
#include "Zenith/Graphics/Shapes/Shapes.hpp"
#include "Zenith/Logging/Logger.hpp"
#include "Zenith/Platform/Event.hpp"
#include "Zenith/Platform/OpenGl/GlDebug.hpp"
#include "Zenith/Platform/OpenGl/Shader.hpp"
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
    glEnable(GL_CULL_FACE);

    set_clear_color(colors::transparent);

    shaders::load_shaders();
    meshes::load_meshes();

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

auto Renderer::submit(const Shape3D& shape, const Material& material) -> void
{
    _draw_commands.emplace_back(&shape, &shape.transform(), &material);
}

auto Renderer::submit(const Drawable& drawable, const glm::mat4& transform, const Material& material) -> void
{
    _draw_commands.emplace_back(&drawable, &transform, &material);
}

auto Renderer::draw(const CubeShape& cube) -> void
{
    auto& mesh = cube.mesh();
    draw(mesh.vertex_array());
}

auto Renderer::draw(const Mesh& mesh) -> void
{
    draw(mesh.vertex_array());
}

auto Renderer::draw(const VertexArray& vertex_array) -> void
{
    vertex_array.bind();
    glDrawElements(GL_TRIANGLES, vertex_array.count(), vertex_array.index_type(), nullptr);
}

auto Renderer::render() -> void
{
    // TODO: batching

    for (const auto& draw_command : _draw_commands)
        execute(draw_command);

    _draw_commands.clear();
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

auto Renderer::execute(const DrawCommand& draw_command) -> void
{
    const auto& [drawable, transform, material] = draw_command;
    const auto& shader = material->shader();

    material->bind();

    ZTH_ASSERT(_camera != nullptr);
    shader->set_unif("viewProjection", _camera->view_projection());
    shader->set_unif("transform", transform ? *transform : glm::mat4{ 1.0f });

    draw_command.drawable->draw();
}

} // namespace zth
