#pragma once

#include <glm/vec2.hpp>

#include "Zenith/Platform/Key.hpp"
#include "Zenith/Platform/Mouse.hpp"
#include "Zenith/Platform/WindowProps.hpp"

namespace zth {

struct WindowResizedEvent
{
    WindowSize new_size;
};

struct KeyPressedEvent
{
    Key key;
};

struct KeyReleasedEvent
{
    Key key;
};

struct MouseButtonPressedEvent
{
    MouseButton button;
};

struct MouseButtonReleasedEvent
{
    MouseButton button;
};

struct MouseMovedEvent
{
    glm::vec2 new_pos;
};

struct MouseWheelScrolledEvent
{
    glm::vec2 delta;
};

} // namespace zth
