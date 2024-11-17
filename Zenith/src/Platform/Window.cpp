#include "Zenith/Platform/Window.hpp"

#include "Zenith/Core/Exception.hpp"
#include "Zenith/Logging/Logger.hpp"
#include "Zenith/Platform/Input.hpp"

namespace zth {

namespace {

bool glfw_initialized = false;
bool glad_initialized = false;

auto ensure_glfw_initialized() -> bool
{
    if (glfw_initialized)
        return true;

    glfw_initialized = glfwInit();
    return glfw_initialized;
}

auto terminate_glfw() -> void
{
    if (!glfw_initialized)
        return;

    glfwTerminate();
    glfw_initialized = false;
}

auto ensure_glad_initialized() -> bool
{
    if (glad_initialized)
        return true;

    glad_initialized = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
    return glad_initialized;
}

} // namespace

Window::Window(const WindowSpec& spec)
{
    if (!ensure_glfw_initialized())
    {
        auto error_message = "Failed to initialize GLFW.";
        ZTH_CORE_CRITICAL(error_message);
        throw Exception(error_message);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, static_cast<int>(spec.gl_version.major));
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, static_cast<int>(spec.gl_version.minor));
    glfwWindowHint(GLFW_OPENGL_PROFILE, static_cast<int>(to_gl_enum(spec.gl_profile)));

    _window = glfwCreateWindow(static_cast<int>(spec.width), static_cast<int>(spec.height), spec.title.data(), nullptr,
                               nullptr);

    if (!_window)
    {
        auto error_message = "Failed to create a window.";
        ZTH_CORE_CRITICAL(error_message);

        if (_window_count == 0)
            terminate_glfw();

        throw Exception(error_message);
    }

    set_active();
    set_vsync(spec.vsync);

    if (!ensure_glad_initialized())
    {
        auto error_message = "Failed to initialize glad.";
        ZTH_CORE_CRITICAL(error_message);

        if (_window_count == 0)
        {
            glfwDestroyWindow(_window);
            terminate_glfw();
        }

        throw Exception(error_message);
    }

    glViewport(0, 0, static_cast<GLsizei>(spec.width), static_cast<GLsizei>(spec.height));

    set_glfw_resize_callback([]([[maybe_unused]] GLFWwindow* window, int new_width, int new_height) {
        glViewport(0, 0, new_width, new_height);
    });

    set_glfw_key_callback([]([[maybe_unused]] GLFWwindow* window, int key, [[maybe_unused]] int scancode, int action,
                             [[maybe_unused]] int mods) {
        auto pressed = false;

        if (action == GLFW_PRESS || action == GLFW_REPEAT)
            pressed = true;
        else if (action == GLFW_RELEASE)
            pressed = false;

        Input::set_key_pressed(glfw_key_to_key(key), pressed);
    });

    _window_count++;
}

Window::~Window()
{
    if (!_window)
        return;

    glfwDestroyWindow(_window);
    _window_count--;

    if (_window_count == 0)
        terminate_glfw();
}

auto Window::set_glfw_resize_callback(ResizeCallback callback) const -> void
{
    glfwSetFramebufferSizeCallback(_window, callback);
}

auto Window::set_glfw_key_callback(KeyCallback callback) const -> void
{
    glfwSetKeyCallback(_window, callback);
}

} // namespace zth
