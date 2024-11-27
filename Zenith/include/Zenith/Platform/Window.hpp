#pragma once

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <GLFW/glfw3.h>

#include <string>

#include "Zenith/Core/Typedefs.hpp"
#include "Zenith/Platform/OpenGl/GlContext.hpp"
#include "Zenith/Utility/Utility.hpp"

namespace zth {

struct WindowSize
{
    u32 width;
    u32 height;
};

struct WindowSpec
{
    u32 width = 800;
    u32 height = 600;
    std::string title = "Title";
    GlVersion gl_version = { 3, 3 };
    GlProfile gl_profile = GlProfile::Compatibility;
    bool fullscreen = false;
    bool vsync = true;
};

class Window
{
public:
    using ResizeCallback = void (*)(GLFWwindow* window, int new_width, int new_height);
    using KeyCallback = void (*)(GLFWwindow* window, int key, int scancode, int action, int mods);

    explicit Window(const WindowSpec& spec = {});
    ZTH_NO_COPY_NO_MOVE(Window)
    ~Window();

    [[nodiscard]] auto should_close() const -> bool;

    auto set_active() const -> void;
    auto set_vsync(bool value) const -> void;
    auto swap_buffers() const -> void;
    auto poll_events() const -> void;

    [[nodiscard]] auto size() const -> WindowSize;
    [[nodiscard]] auto mouse_pos() const -> glm::vec2;

private:
    GLFWwindow* _window = nullptr;
    static inline bool _window_exists = false;

private:
    auto set_glfw_resize_callback(ResizeCallback callback) const -> void;
    auto set_glfw_key_callback(KeyCallback callback) const -> void;
};

} // namespace zth
