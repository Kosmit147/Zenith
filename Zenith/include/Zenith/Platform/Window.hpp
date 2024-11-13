#pragma once

#include <GLFW/glfw3.h>

#include <optional>
#include <string>

#include "Zenith/Core/Typedefs.hpp"
#include "Zenith/Core/ZthApi.hpp"
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

class ZTH_API Window
{
public:
    [[nodiscard]] static auto create(const WindowSpec& spec = {}) -> std::optional<Window>;

    ZTH_NO_COPY(Window)

    Window(Window&& other) noexcept;
    auto operator=(Window&& other) noexcept -> Window&;

    ~Window();

    [[nodiscard]] auto should_close() const -> bool { return glfwWindowShouldClose(_window); }

    auto set_active() const -> void { glfwMakeContextCurrent(_window); }
    auto swap_buffers() const -> void { glfwSwapBuffers(_window); }
    auto poll_events() const -> void { glfwPollEvents(); }
    auto set_vsync(bool value) const -> void { glfwSwapInterval(value); }

private:
    GLFWwindow* _window = nullptr;
    static inline u32 _window_count = 0;

private:
    explicit Window(const WindowSpec& spec);
};

} // namespace zth
