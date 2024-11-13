#pragma once

#include <GLFW/glfw3.h>

#include <string>

#include "Zenith/Core/Typedefs.hpp"
#include "Zenith/Core/ZthApi.hpp"
#include "Zenith/Platform/OpenGl/GlContext.hpp"

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

class ZTH_API Window
{
public:
    explicit Window(const WindowSpec& spec = {});
    ~Window();

    [[nodiscard]] auto should_close() const -> bool { return glfwWindowShouldClose(_window); }

    auto make_context_current() const -> void { glfwMakeContextCurrent(_window); }
    auto swap_buffers() const -> void { glfwSwapBuffers(_window); }
    auto poll_events() const -> void { glfwPollEvents(); }
    auto set_vsync(bool value) const -> void { glfwSwapInterval(value); }

private:
    GLFWwindow* _window = nullptr;

    static inline u32 _window_count = 0;
};

} // namespace zth
