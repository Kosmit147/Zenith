#include "Zenith/Platform/Window.hpp"

#include "Zenith/Core/Exception.hpp"
#include "Zenith/Logging/Logger.hpp"
#include "Zenith/Platform/Input.hpp"

namespace zth {

namespace {

[[nodiscard]] auto init_glad() -> bool
{
    return gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
}

[[nodiscard]] auto create_glfw_window(u32 width, u32 height, const char* title, bool fullscreen) -> GLFWwindow*
{
    GLFWmonitor* monitor = nullptr;

    if (fullscreen)
        monitor = glfwGetPrimaryMonitor();

    return glfwCreateWindow(static_cast<int>(width), static_cast<int>(height), title, monitor, nullptr);
}

} // namespace

Window::Window(const WindowSpec& spec)
{
    if (_window_exists)
    {
        auto error_message = "Tried to create a new window, but a window already exists.";
        ZTH_CORE_CRITICAL(error_message);
        throw Exception(error_message);
    }

    if (!glfwInit())
    {
        auto error_message = "Failed to initialize GLFW.";
        ZTH_CORE_CRITICAL(error_message);
        throw Exception(error_message);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, static_cast<int>(spec.gl_version.major));
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, static_cast<int>(spec.gl_version.minor));
    glfwWindowHint(GLFW_OPENGL_PROFILE, static_cast<int>(to_gl_enum(spec.gl_profile)));

#ifdef _DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

    _window = create_glfw_window(spec.width, spec.height, spec.title.data(), spec.fullscreen);

    if (!_window)
    {
        auto error_message = "Failed to create a window.";
        ZTH_CORE_CRITICAL(error_message);
        glfwTerminate();
        throw Exception(error_message);
    }

    ZTH_CORE_INFO("Window initialized.");

    set_active();
    set_vsync(spec.vsync);

    if (!init_glad())
    {
        auto error_message = "Failed to initialize glad.";
        ZTH_CORE_CRITICAL(error_message);
        glfwDestroyWindow(_window);
        glfwTerminate();
        throw Exception(error_message);
    }

    {
        auto vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
        auto renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
        auto version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
        auto glsl_version = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));

        ZTH_CORE_INFO("Initialized OpenGL context:\n"
                      "\tVendor: {}\n"
                      "\tRenderer: {}\n"
                      "\tVersion: {}\n"
                      "\tGLSL Version: {}",
                      vendor, renderer, version, glsl_version);
    }

#ifdef _DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(
        []([[maybe_unused]] GLenum source, GLenum type, [[maybe_unused]] GLuint id, GLenum severity,
           [[maybe_unused]] GLsizei length, const GLchar* message, [[maybe_unused]] const void* userParam) {
            auto type_str = [=] {
                switch (type)
                {
                case GL_DEBUG_TYPE_ERROR:
                    return "Error";
                case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                    return "Deprecated behavior";
                case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                    return "Undefined behavior";
                case GL_DEBUG_TYPE_PORTABILITY:
                    return "Portability";
                case GL_DEBUG_TYPE_PERFORMANCE:
                    return "Performance";
                case GL_DEBUG_TYPE_MARKER:
                    return "Marker";
                case GL_DEBUG_TYPE_OTHER:
                    return "Other";
                default:
                    return "";
                }
            }();

            switch (severity)
            {
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                ZTH_CORE_INFO("[OpenGL] {}: {}", type_str, message);
                break;
            case GL_DEBUG_SEVERITY_LOW:
                ZTH_CORE_WARN("[OpenGL] {}: {}", type_str, message);
                break;
            case GL_DEBUG_SEVERITY_MEDIUM:
            case GL_DEBUG_SEVERITY_HIGH:
                ZTH_CORE_ERROR("[OpenGL] {}: {}", type_str, message);
                break;
            default:
                break;
            }
        },
        nullptr);
#endif

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

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

    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // a solution for sudden movements caused by the first read of mouse position
    Input::set_mouse_pos(mouse_pos());
    Input::set_mouse_pos(mouse_pos());

    _window_exists = true;
}

Window::~Window()
{
    glfwDestroyWindow(_window);
    glfwTerminate();
    _window_exists = false;
}

auto Window::should_close() const -> bool
{
    return glfwWindowShouldClose(_window);
}

auto Window::set_active() const -> void
{
    glfwMakeContextCurrent(_window);
}

auto Window::set_vsync(bool value) const -> void
{
    glfwSwapInterval(value);
}

auto Window::swap_buffers() const -> void
{
    glfwSwapBuffers(_window);
}

auto Window::poll_events() const -> void
{
    glfwPollEvents();
    Input::set_mouse_pos(mouse_pos());
}

auto Window::size() const -> WindowSize
{
    int width, height;
    glfwGetWindowSize(_window, &width, &height);
    return { .width = static_cast<u32>(width), .height = static_cast<u32>(height) };
}

auto Window::mouse_pos() const -> glm::vec2
{
    double x_pos, y_pos;
    glfwGetCursorPos(_window, &x_pos, &y_pos);
    return { static_cast<float>(x_pos), static_cast<float>(y_pos) };
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
