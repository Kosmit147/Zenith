#include "Zenith/Platform/Window.hpp"

namespace zth {

namespace {

bool glfw_initialized = false;

auto init_glfw() -> bool
{
    if (glfw_initialized)
        return true;

    if (!glfwInit())
        return false;

    glfw_initialized = true;
    return true;
}

auto terminate_glfw() -> void
{
    if (!glfw_initialized)
        return;

    glfwTerminate();
    glfw_initialized = false;
}

} // namespace

Window::Window(const WindowSpec& spec)
{
    if (!init_glfw())
    {
        std::println(std::cerr, "GLFW failed to initialize.");
        throw std::runtime_error("GLFW failed to initialize.");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, static_cast<int>(spec.gl_version.major));
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, static_cast<int>(spec.gl_version.minor));
    glfwWindowHint(GLFW_OPENGL_PROFILE, static_cast<int>(to_gl_enum(spec.gl_profile)));

    _window = glfwCreateWindow(static_cast<int>(spec.width), static_cast<int>(spec.height), spec.title.data(), nullptr,
                               nullptr);

    if (!_window)
    {
        if (_window_count == 0)
            terminate_glfw();

        std::println(std::cerr, "Failed to create a window.");
        throw std::runtime_error("Failed to create a window.");
    }

    make_context_current();
    set_vsync(spec.vsync);

    _window_count++;
}

Window::~Window()
{
    glfwDestroyWindow(_window);
    _window_count--;

    if (_window_count == 0)
        terminate_glfw();
}

} // namespace zth
