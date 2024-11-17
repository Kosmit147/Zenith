#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>

#include "Zenith/Core/Typedefs.hpp"
#include "Zenith/Platform/OpenGl/GlContext.hpp"
#include "Zenith/Utility/Utility.hpp"

namespace zth {

struct WindowSpec
{
    u32 width = 800;
    u32 height = 600;
    std::string title = "Title";
    GlVersion gl_version = { 3, 3 };
    GlProfile gl_profile = GlProfile::Compatibility;
    bool vsync = true;
};

class Window
{
public:
    using OnResizeCallback = void (*)(GLFWwindow* window, int new_width, int new_height);

    explicit Window(const WindowSpec& spec = {});

    ZTH_NO_COPY_NO_MOVE(Window)

    ~Window();

    [[nodiscard]] auto should_close() const -> bool { return glfwWindowShouldClose(_window); }

    auto set_active() const -> void { glfwMakeContextCurrent(_window); }
    auto swap_buffers() const -> void { glfwSwapBuffers(_window); }
    auto poll_events() const -> void { glfwPollEvents(); }
    auto set_vsync(bool value) const -> void { glfwSwapInterval(value); }
    auto set_resize_callback(OnResizeCallback callback) const -> void;

private:
    GLFWwindow* _window = nullptr;
    static inline u32 _window_count = 0;
};

} // namespace zth
