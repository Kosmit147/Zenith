#include "Zenith/Platform/Mouse.hpp"

#include "Zenith/Core/Assert.hpp"

namespace zth {

auto glfw_mouse_button_to_mouse_button(int button) -> MouseButton
{
    switch (button)
    {
        using enum MouseButton;
    case GLFW_MOUSE_BUTTON_LEFT:
        return Left;
    case GLFW_MOUSE_BUTTON_MIDDLE:
        return Middle;
    case GLFW_MOUSE_BUTTON_RIGHT:
        return Right;
    default:
        break;
    }

    ZTH_ASSERT(false);
    std::unreachable();
}

auto to_string(MouseButton key) -> const char*
{
    switch (key)
    {
        using enum MouseButton;
    case Left:
        return "Left";
    case Middle:
        return "Middle";
    case Right:
        return "Right";
    }

    ZTH_ASSERT(false);
    return "Unknown";
}

} // namespace zth
