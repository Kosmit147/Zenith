#pragma once

#include <glm/vec2.hpp>

#include "zenith/platform/key.hpp"
#include "zenith/platform/mouse.hpp"

namespace zth {

struct WindowResizedEvent
{
    glm::uvec2 new_size;
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
