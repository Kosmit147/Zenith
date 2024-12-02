#include "Zenith/Platform/Window.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Zenith/Core/Assert.hpp"
#include "Zenith/Core/Exception.hpp"
#include "Zenith/Core/Typedefs.hpp"
#include "Zenith/Logging/Logger.hpp"
#include "Zenith/Platform/EventQueue.hpp"

namespace zth {

namespace {

[[nodiscard]] auto to_glfw_enum(bool value) -> int
{
    return value ? GLFW_TRUE : GLFW_FALSE;
}

[[nodiscard]] constexpr auto to_glfw_enum(GlProfile profile) -> int
{
    switch (profile)
    {
        using enum GlProfile;
    case Compatibility:
        return GLFW_OPENGL_COMPAT_PROFILE;
    case Core:
        return GLFW_OPENGL_CORE_PROFILE;
    }

    ZTH_ASSERT(false);
    std::unreachable();
}

[[nodiscard]] auto init_glad() -> bool
{
    return gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
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

    set_glfw_window_hints(spec);
    _window = create_glfw_window(spec.size, spec.title.data(), spec.fullscreen);

    if (!_window)
    {
        auto error_message = "Failed to create a window.";
        ZTH_CORE_CRITICAL(error_message);
        glfwTerminate();
        throw Exception(error_message);
    }

    set_active();
    set_vsync(spec.vsync);

    set_glfw_input_callbacks();
    set_glfw_cursor_enabled(spec.cursor_enabled);

    if (spec.forced_aspect_ratio)
        glfw_force_aspect_ratio(spec.forced_aspect_ratio.value());

    if (!init_glad())
    {
        auto error_message = "Failed to initialize glad.";
        ZTH_CORE_CRITICAL(error_message);
        glfwDestroyWindow(_window);
        glfwTerminate();
        throw Exception(error_message);
    }

    // make sure that the renderer sets glViewport after initialization
    EventQueue::push(WindowResizedEvent{ size() });

    ZTH_CORE_INFO("Window initialized.");
}

auto Window::shut_down() -> void
{
    glfwDestroyWindow(_window);
    glfwTerminate();

    ZTH_CORE_INFO("Window shut down.");
}

auto Window::should_close() -> bool
{
    return glfwWindowShouldClose(_window);
}

auto Window::time() -> double
{
    return glfwGetTime();
}

auto Window::set_active() -> void
{
    glfwMakeContextCurrent(_window);
}

auto Window::set_vsync(bool value) -> void
{
    glfwSwapInterval(value);
}

auto Window::swap_buffers() -> void
{
    glfwSwapBuffers(_window);
}

auto Window::poll_events() -> void
{
    glfwPollEvents();
}

auto Window::close() -> void
{
    glfwSetWindowShouldClose(_window, true);
}

auto Window::size() -> Size
{
    int width, height;
    glfwGetWindowSize(_window, &width, &height);
    return { .width = static_cast<u32>(width), .height = static_cast<u32>(height) };
}

auto Window::mouse_pos() -> glm::vec2
{
    double x_pos, y_pos;
    glfwGetCursorPos(_window, &x_pos, &y_pos);
    return { static_cast<float>(x_pos), static_cast<float>(y_pos) };
}

auto Window::create_glfw_window(Size size, const char* title, bool fullscreen) -> GLFWwindow*
{
    GLFWmonitor* monitor = nullptr;

    if (fullscreen)
        monitor = glfwGetPrimaryMonitor();

    return glfwCreateWindow(static_cast<int>(size.width), static_cast<int>(size.height), title, monitor, nullptr);
}

auto Window::set_glfw_window_hints(const WindowSpec& spec) -> void
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, static_cast<int>(spec.gl_version.major));
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, static_cast<int>(spec.gl_version.minor));
    glfwWindowHint(GLFW_OPENGL_PROFILE, to_glfw_enum(spec.gl_profile));
    glfwWindowHint(GLFW_RESIZABLE, to_glfw_enum(spec.resizable));
    glfwWindowHint(GLFW_MAXIMIZED, to_glfw_enum(spec.maximized));
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, to_glfw_enum(spec.transparent_framebuffer));

#ifdef _DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif
}

auto Window::set_glfw_input_callbacks() -> void
{
    set_glfw_resize_callback(glfw_resize_callback);
    set_glfw_key_callback(glfw_key_callback);
    set_glfw_mouse_button_callback(glfw_mouse_button_callback);
    set_glfw_mouse_pos_callback(glfw_mouse_pos_callback);
    set_glfw_scroll_callback(glfw_scroll_callback);
}

auto Window::set_glfw_cursor_enabled(bool enabled) -> void
{
    if (enabled)
        glfw_enable_cursor();
    else
        glfw_disable_cursor();
}

auto Window::glfw_enable_cursor() -> void
{
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(_window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
}

auto Window::glfw_disable_cursor() -> void
{
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}

auto Window::glfw_force_aspect_ratio(WindowAspectRatio aspect_ratio) -> void
{
    auto [num, den] = aspect_ratio;
    glfwSetWindowAspectRatio(_window, static_cast<int>(num), static_cast<int>(den));
}

auto Window::set_glfw_resize_callback(ResizeCallback callback) -> void
{
    glfwSetFramebufferSizeCallback(_window, callback);
}

auto Window::set_glfw_key_callback(KeyCallback callback) -> void
{
    glfwSetKeyCallback(_window, callback);
}

auto Window::set_glfw_mouse_button_callback(MouseButtonCallback callback) -> void
{
    glfwSetMouseButtonCallback(_window, callback);
}

auto Window::set_glfw_mouse_pos_callback(MousePosCallback callback) -> void
{
    glfwSetCursorPosCallback(_window, callback);
}

auto Window::set_glfw_scroll_callback(ScrollCallback callback) -> void
{
    glfwSetScrollCallback(_window, callback);
}

auto Window::glfw_resize_callback([[maybe_unused]] GLFWwindow* window, int new_width, int new_height) -> void
{
    auto new_size = Size{ static_cast<u32>(new_width), static_cast<u32>(new_height) };
    EventQueue::push(WindowResizedEvent{ new_size });
}

auto Window::glfw_key_callback([[maybe_unused]] GLFWwindow* window, int key, [[maybe_unused]] int scancode, int action,
                               [[maybe_unused]] int mods) -> void
{
    if (action == GLFW_PRESS)
        EventQueue::push(KeyPressedEvent{ glfw_key_to_key(key) });
    else if (action == GLFW_RELEASE)
        EventQueue::push(KeyReleasedEvent{ glfw_key_to_key(key) });
}

auto Window::glfw_mouse_button_callback([[maybe_unused]] GLFWwindow* window, int button, int action,
                                        [[maybe_unused]] int mods) -> void
{
    if (action == GLFW_PRESS)
        EventQueue::push(MouseButtonPressedEvent{ glfw_mouse_button_to_mouse_button(button) });
    else if (action == GLFW_RELEASE)
        EventQueue::push(MouseButtonReleasedEvent{ glfw_mouse_button_to_mouse_button(button) });
}

auto Window::glfw_mouse_pos_callback([[maybe_unused]] GLFWwindow* window, double xpos, double ypos) -> void
{
    auto new_pos = glm::vec2{ static_cast<float>(xpos), static_cast<float>(ypos) };
    EventQueue::push(MouseMovedEvent{ new_pos });
}

auto Window::glfw_scroll_callback([[maybe_unused]] GLFWwindow* window, double xoffset, double yoffset) -> void
{
    auto delta = glm::vec2{ static_cast<float>(xoffset), static_cast<float>(yoffset) };
    EventQueue::push(MouseWheelScrolledEvent{ delta });
}

} // namespace zth
