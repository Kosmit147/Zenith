#include "zenith/system/window.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "zenith/core/assert.hpp"
#include "zenith/core/exception.hpp"
#include "zenith/core/typedefs.hpp"
#include "zenith/log/format.hpp"
#include "zenith/log/logger.hpp"
#include "zenith/system/event_queue.hpp"
#include "zenith/util/defer.hpp"

namespace zth {

namespace {

[[nodiscard]] auto to_glfw_enum(bool value) -> int
{
    return value ? GLFW_TRUE : GLFW_FALSE;
}

[[nodiscard]] auto to_glfw_enum(gl::Profile profile) -> int
{
    switch (profile)
    {
        using enum gl::Profile;
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
    ZTH_CORE_INFO("Initializing window...");

    if (spec.gl_version != gl::Version{ 4, 6 })
    {
        auto error_message = ZTH_FORMAT(
            "Zenith supports only OpenGL 4.6. Tried to create an OpenGL context with version {}.", spec.gl_version);
        ZTH_CORE_CRITICAL(error_message);
        throw Exception{ error_message };
    }

    if (spec.gl_profile != gl::Profile::Core)
    {
        auto error_message =
            ZTH_FORMAT("Zenith supports only OpenGL Core profile. Tried to create an OpenGL context with {} profile.",
                       spec.gl_profile);
        ZTH_CORE_CRITICAL(error_message);
        throw Exception{ error_message };
    }

    if (!glfwInit())
    {
        auto error_message = "Failed to initialize GLFW.";
        ZTH_CORE_CRITICAL(error_message);
        throw Exception{ error_message };
    }

    Defer terminate_glfw{ [] { glfwTerminate(); } };

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, static_cast<int>(spec.gl_version.major));
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, static_cast<int>(spec.gl_version.minor));
    glfwWindowHint(GLFW_OPENGL_PROFILE, to_glfw_enum(spec.gl_profile));
    glfwWindowHint(GLFW_RESIZABLE, to_glfw_enum(spec.resizable));
    glfwWindowHint(GLFW_MAXIMIZED, to_glfw_enum(spec.maximized));
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, to_glfw_enum(spec.transparent_framebuffer));

#if !defined(ZTH_DIST_BUILD)
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

    _window = create_glfw_window(spec.size, spec.title.data(), spec.fullscreen);

    if (!_window)
    {
        auto error_message = "Failed to create a window.";
        ZTH_CORE_CRITICAL(error_message);
        throw Exception{ error_message };
    }

    Defer destroy_window{ [&] { glfwDestroyWindow(_window); } };

    set_active();
    set_vsync(spec.vsync);

    if (spec.frame_rate_limit)
        set_frame_rate_limit(*spec.frame_rate_limit);

    set_cursor_enabled(spec.cursor_enabled);

    set_glfw_resize_callback(glfw_resize_callback);
    set_glfw_key_callback(glfw_key_callback);
    set_glfw_mouse_button_callback(glfw_mouse_button_callback);
    set_glfw_mouse_pos_callback(glfw_mouse_pos_callback);
    set_glfw_scroll_callback(glfw_scroll_callback);

    if (spec.forced_aspect_ratio)
        glfw_force_aspect_ratio(*spec.forced_aspect_ratio);

    if (!init_glad())
    {
        auto error_message = "Failed to initialize glad.";
        ZTH_CORE_CRITICAL(error_message);
        throw Exception{ error_message };
    }

    // make sure that the renderer sets glViewport after initialization
    EventQueue::push(WindowResizedEvent{ size() });

    destroy_window.dismiss();
    terminate_glfw.dismiss();

    ZTH_CORE_INFO("Window initialized.");
}

auto Window::shut_down() -> void
{
    ZTH_CORE_INFO("Shutting down window...");

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

    auto frame_time = time() - _last_frame_time;

    // @todo: Sleep instead if spinning.
    while (frame_time < _target_frame_time)
        frame_time = time() - _last_frame_time;

    _last_frame_time = time();
}

auto Window::poll_events() -> void
{
    glfwPollEvents();
}

auto Window::close() -> void
{
    glfwSetWindowShouldClose(_window, true);
}

auto Window::set_frame_rate_limit(u32 limit) -> void
{
    _frame_rate_limit = limit;
    _target_frame_time = 1.0 / static_cast<double>(limit);
}

auto Window::disable_frame_rate_limit() -> void
{
    _frame_rate_limit = nil;
    _target_frame_time = 0.0;
}

auto Window::set_cursor_enabled(bool enabled) -> void
{
    if (enabled)
    {
        glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        if (glfwRawMouseMotionSupported())
            glfwSetInputMode(_window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
    }
    else
    {
        glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        if (glfwRawMouseMotionSupported())
            glfwSetInputMode(_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }
}

auto Window::glfw_handle() -> GLFWwindow*
{
    return _window;
}

auto Window::size() -> glm::uvec2
{
    int width, height;
    glfwGetWindowSize(_window, &width, &height);
    return glm::uvec2{ static_cast<unsigned int>(width), static_cast<unsigned int>(height) };
}

auto Window::mouse_pos() -> glm::vec2
{
    double x_pos, y_pos;
    glfwGetCursorPos(_window, &x_pos, &y_pos);
    return glm::vec2{ static_cast<float>(x_pos), static_cast<float>(y_pos) };
}

auto Window::frame_rate_limit() -> Optional<u32>
{
    return _frame_rate_limit;
}

auto Window::cursor_enabled() -> bool
{
    auto mode = glfwGetInputMode(_window, GLFW_CURSOR);

    if (mode == GLFW_CURSOR_NORMAL)
        return true;
    else if (mode == GLFW_CURSOR_DISABLED)
        return false;

    ZTH_ASSERT(false);
    return false;
}

auto Window::create_glfw_window(glm::uvec2 size, const char* title, bool fullscreen) -> GLFWwindow*
{
    GLFWmonitor* monitor = nullptr;

    if (fullscreen)
        monitor = glfwGetPrimaryMonitor();

    return glfwCreateWindow(static_cast<int>(size.x), static_cast<int>(size.y), title, monitor, nullptr);
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
    auto new_size = glm::uvec2{ static_cast<unsigned int>(new_width), static_cast<unsigned int>(new_height) };
    EventQueue::push(WindowResizedEvent{ new_size });
}

auto Window::glfw_key_callback([[maybe_unused]] GLFWwindow* window, int key, [[maybe_unused]] int scancode, int action,
                               [[maybe_unused]] int mods) -> void
{
    if (auto converted_key = glfw_key_to_key(key))
    {
        if (action == GLFW_PRESS)
            EventQueue::push(KeyPressedEvent{ *converted_key });
        else if (action == GLFW_RELEASE)
            EventQueue::push(KeyReleasedEvent{ *converted_key });
    }
}

auto Window::glfw_mouse_button_callback([[maybe_unused]] GLFWwindow* window, int button, int action,
                                        [[maybe_unused]] int mods) -> void
{
    if (auto converted_mouse_button = glfw_mouse_button_to_mouse_button(button))
    {
        if (action == GLFW_PRESS)
            EventQueue::push(MouseButtonPressedEvent{ *converted_mouse_button });
        else if (action == GLFW_RELEASE)
            EventQueue::push(MouseButtonReleasedEvent{ *converted_mouse_button });
    }
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
