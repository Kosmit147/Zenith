#pragma once

#include <glm/vec2.hpp>

#include "zenith/gl/context.hpp"
#include "zenith/stl/string.hpp"
#include "zenith/util/optional.hpp"

struct GLFWwindow;

namespace zth {

struct WindowAspectRatio
{
    u32 numerator;
    u32 denominator;
};

struct WindowSpec
{
    glm::uvec2 size = { 800, 600 };
    String title = "Title";
    gl::Version gl_version = { 4, 6 };
    gl::Profile gl_profile = gl::Profile::Core;
    bool fullscreen = false;
    bool vsync = true;
    Optional<u32> frame_rate_limit = nil;
    bool resizable = true;
    bool maximized = false;
    bool cursor_enabled = false;
    bool transparent_framebuffer = false;
    Optional<WindowAspectRatio> forced_aspect_ratio = nil;
};

class Window
{
public:
    using ResizeCallback = void (*)(GLFWwindow* window, int new_width, int new_height);
    using KeyCallback = void (*)(GLFWwindow* window, int key, int scancode, int action, int mods);
    using MouseButtonCallback = void (*)(GLFWwindow* window, int button, int action, int mods);
    using MousePosCallback = void (*)(GLFWwindow* window, double xpos, double ypos);
    using ScrollCallback = void (*)(GLFWwindow* window, double xoffset, double yoffset);

    Window() = delete;

    static auto init(const WindowSpec& spec) -> void;
    static auto shut_down() -> void;

    [[nodiscard]] static auto should_close() -> bool;

    // Returns the time since the window was created, in seconds.
    [[nodiscard]] static auto time() -> double;

    static auto make_context_current() -> void;
    static auto set_vsync_enabled(bool enabled) -> void;
    static auto swap_buffers() -> void;
    static auto poll_events() -> void;
    static auto close() -> void;

    static auto set_frame_rate_limit(u32 limit) -> void;
    static auto disable_frame_rate_limit() -> void;
    static auto set_cursor_enabled(bool enabled) -> void;

    [[nodiscard]] static auto glfw_handle() -> GLFWwindow*;
    [[nodiscard]] static auto size() -> glm::uvec2;
    [[nodiscard]] static auto mouse_pos() -> glm::vec2;
    [[nodiscard]] static auto frame_rate_limit() -> Optional<u32>;
    [[nodiscard]] static auto cursor_enabled() -> bool;

private:
    static inline GLFWwindow* _window = nullptr;
    static inline double _target_frame_time = 0.0;
    static inline double _last_frame_time = 0.0;
    static inline Optional<u32> _frame_rate_limit = nil;

private:
    [[nodiscard]] static auto create_glfw_window(glm::uvec2 size, const char* title, bool fullscreen) -> GLFWwindow*;
    static auto glfw_force_aspect_ratio(WindowAspectRatio aspect_ratio) -> void;

    static auto set_glfw_resize_callback(ResizeCallback callback) -> void;
    static auto set_glfw_key_callback(KeyCallback callback) -> void;
    static auto set_glfw_mouse_button_callback(MouseButtonCallback callback) -> void;
    static auto set_glfw_mouse_pos_callback(MousePosCallback callback) -> void;
    static auto set_glfw_scroll_callback(ScrollCallback callback) -> void;

    static auto glfw_resize_callback(GLFWwindow* window, int new_width, int new_height) -> void;
    static auto glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) -> void;
    static auto glfw_mouse_button_callback(GLFWwindow* window, int button, int action, int mods) -> void;
    static auto glfw_mouse_pos_callback(GLFWwindow* window, double xpos, double ypos) -> void;
    static auto glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) -> void;
};

} // namespace zth
