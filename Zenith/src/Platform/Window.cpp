#include "Zenith/Platform/Window.hpp"

#include "Zenith/Core/Exception.hpp"
#include "Zenith/Core/Typedefs.hpp"
#include "Zenith/Logging/Logger.hpp"
#include "Zenith/Platform/EventQueue.hpp"

namespace zth {

namespace {

GLFWwindow* window = nullptr;

[[nodiscard]] auto to_glfw_enum(bool value) -> int
{
    return value ? GLFW_TRUE : GLFW_FALSE;
}

[[nodiscard]] auto init_glad() -> bool
{
    return gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
}

[[nodiscard]] auto create_glfw_window(WindowSize size, const char* title, bool fullscreen) -> GLFWwindow*
{
    GLFWmonitor* monitor = nullptr;

    if (fullscreen)
        monitor = glfwGetPrimaryMonitor();

    return glfwCreateWindow(static_cast<int>(size.width), static_cast<int>(size.height), title, monitor, nullptr);
}

} // namespace

auto Window::init(const WindowSpec& spec) -> void
{
    if (!glfwInit())
    {
        auto error_message = "Failed to initialize GLFW.";
        ZTH_CORE_CRITICAL(error_message);
        throw Exception(error_message);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, static_cast<int>(spec.gl_version.major));
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, static_cast<int>(spec.gl_version.minor));
    glfwWindowHint(GLFW_OPENGL_PROFILE, to_glfw_enum(spec.gl_profile));
    glfwWindowHint(GLFW_RESIZABLE, to_glfw_enum(spec.resizable));
    glfwWindowHint(GLFW_MAXIMIZED, to_glfw_enum(spec.maximized));
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, to_glfw_enum(spec.transparent_framebuffer));

#ifdef _DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

    window = create_glfw_window(spec.size, spec.title.data(), spec.fullscreen);

    if (!window)
    {
        auto error_message = "Failed to create a window.";
        ZTH_CORE_CRITICAL(error_message);
        glfwTerminate();
        throw Exception(error_message);
    }

    ZTH_CORE_INFO("Window initialized.");

    set_active();
    set_vsync(spec.vsync);

    set_glfw_resize_callback([](GLFWwindow*, int new_width, int new_height) {
        auto new_size = WindowSize{ static_cast<u32>(new_width), static_cast<u32>(new_height) };
        EventQueue::push(WindowResizedEvent{ new_size });
    });

    set_glfw_key_callback([](GLFWwindow*, int key, int, int action, int) {
        if (action == GLFW_PRESS)
            EventQueue::push(KeyPressedEvent{ glfw_key_to_key(key) });
        else if (action == GLFW_RELEASE)
            EventQueue::push(KeyReleasedEvent{ glfw_key_to_key(key) });
    });

    set_glfw_mouse_button_callback([](GLFWwindow*, int button, int action, int) {
        if (action == GLFW_PRESS)
            EventQueue::push(MouseButtonPressedEvent{ glfw_mouse_button_to_mouse_button(button) });
        else if (action == GLFW_RELEASE)
            EventQueue::push(MouseButtonReleasedEvent{ glfw_mouse_button_to_mouse_button(button) });
    });

    set_glfw_mouse_pos_callback([](GLFWwindow*, double xpos, double ypos) {
        auto new_pos = glm::vec2{ static_cast<float>(xpos), static_cast<float>(ypos) };
        EventQueue::push(MouseMovedEvent{ new_pos });
    });

    set_glfw_scroll_callback([](GLFWwindow*, double xoffset, double yoffset) {
        auto delta = glm::vec2{ static_cast<float>(xoffset), static_cast<float>(yoffset) };
        EventQueue::push(MouseWheelScrolledEvent{ delta });
    });

    if (!spec.cursor_enabled)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        if (glfwRawMouseMotionSupported())
            glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }

    if (spec.forced_aspect_ratio)
    {
        auto [num, den] = spec.forced_aspect_ratio.value();
        glfwSetWindowAspectRatio(window, static_cast<int>(num), static_cast<int>(den));
    }

    if (!init_glad())
    {
        auto error_message = "Failed to initialize glad.";
        ZTH_CORE_CRITICAL(error_message);
        glfwDestroyWindow(window);
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

    EventQueue::push(WindowResizedEvent{ size() });
}

auto Window::shut_down() -> void
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

auto Window::should_close() -> bool
{
    return glfwWindowShouldClose(window);
}

auto Window::time() -> double
{
    return glfwGetTime();
}

auto Window::set_active() -> void
{
    glfwMakeContextCurrent(window);
}

auto Window::set_vsync(bool value) -> void
{
    glfwSwapInterval(value);
}

auto Window::swap_buffers() -> void
{
    glfwSwapBuffers(window);
}

auto Window::poll_events() -> void
{
    glfwPollEvents();
}

auto Window::close() -> void
{
    glfwSetWindowShouldClose(window, true);
}

auto Window::size() -> WindowSize
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    return { .width = static_cast<u32>(width), .height = static_cast<u32>(height) };
}

auto Window::mouse_pos() -> glm::vec2
{
    double x_pos, y_pos;
    glfwGetCursorPos(window, &x_pos, &y_pos);
    return { static_cast<float>(x_pos), static_cast<float>(y_pos) };
}

auto Window::set_glfw_resize_callback(ResizeCallback callback) -> void
{
    glfwSetFramebufferSizeCallback(window, callback);
}

auto Window::set_glfw_key_callback(KeyCallback callback) -> void
{
    glfwSetKeyCallback(window, callback);
}

auto Window::set_glfw_mouse_button_callback(MouseButtonCallback callback) -> void
{
    glfwSetMouseButtonCallback(window, callback);
}

auto Window::set_glfw_mouse_pos_callback(MousePosCallback callback) -> void
{
    glfwSetCursorPosCallback(window, callback);
}

auto Window::set_glfw_scroll_callback(ScrollCallback callback) -> void
{
    glfwSetScrollCallback(window, callback);
}

} // namespace zth
