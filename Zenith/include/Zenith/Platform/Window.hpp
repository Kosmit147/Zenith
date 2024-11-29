#pragma once

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <GLFW/glfw3.h>

#include <optional>
#include <string>

#include "Zenith/Platform/OpenGl/GlContext.hpp"
#include "Zenith/Platform/WindowProps.hpp"

namespace zth {

struct WindowSpec
{
    WindowSize size = { .width = 800, .height = 600 };
    std::string title = "Title";
    GlVersion gl_version = { 3, 3 };
    GlProfile gl_profile = GlProfile::Compatibility;
    bool fullscreen = false;
    bool vsync = true;
    bool resizable = true;
    bool maximized = false;
    bool cursor_enabled = false;
    bool transparent_framebuffer = false;
    std::optional<WindowAspectRatio> forced_aspect_ratio = std::nullopt;
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
    [[nodiscard]] static auto time() -> double;

    static auto set_active() -> void;
    static auto set_vsync(bool value) -> void;
    static auto swap_buffers() -> void;
    static auto poll_events() -> void;
    static auto close() -> void;

    [[nodiscard]] static auto size() -> WindowSize;
    [[nodiscard]] static auto mouse_pos() -> glm::vec2;

private:
    static auto set_glfw_resize_callback(ResizeCallback callback) -> void;
    static auto set_glfw_key_callback(KeyCallback callback) -> void;
    static auto set_glfw_mouse_button_callback(MouseButtonCallback callback) -> void;
    static auto set_glfw_mouse_pos_callback(MousePosCallback callback) -> void;
    static auto set_glfw_scroll_callback(ScrollCallback callback) -> void;
};

} // namespace zth
