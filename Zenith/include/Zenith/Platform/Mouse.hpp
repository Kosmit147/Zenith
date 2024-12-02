#pragma once

#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>

#include <array>
#include <string>

#include "Zenith/Core/Typedefs.hpp"

namespace zth {

enum class MouseButton : u8
{
    Left = GLFW_MOUSE_BUTTON_LEFT,
    Middle = GLFW_MOUSE_BUTTON_MIDDLE,
    Right = GLFW_MOUSE_BUTTON_RIGHT,
};

constexpr std::array mouse_button_enumerations = {
    MouseButton::Left,
    MouseButton::Middle,
    MouseButton::Right,
};

[[nodiscard]] auto glfw_mouse_button_to_mouse_button(int button) -> MouseButton;
[[nodiscard]] auto to_string(MouseButton key) -> const char*;

} // namespace zth

template<> struct fmt::formatter<zth::MouseButton> : formatter<std::string>
{
    static auto format(zth::MouseButton mouse_button, format_context& ctx) -> decltype(ctx.out())
    {
        return format_to(ctx.out(), "{}", zth::to_string(mouse_button));
    }
};
