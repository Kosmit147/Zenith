#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <array>

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
