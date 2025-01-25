#pragma once

#include "zenith/core/typedefs.hpp"
#include "zenith/platform/events.hpp"

namespace zth {

enum class EventType : u8
{
    WindowResized,
    KeyPressed,
    KeyReleased,
    MouseButtonPressed,
    MouseButtonReleased,
    MouseMoved,
    MouseWheelScrolled,
};

enum class EventCategory : u8
{
    WindowEvent,
    InputEvent,
};

class Event
{
public:
    Event(WindowResizedEvent event);
    Event(KeyPressedEvent event);
    Event(KeyReleasedEvent event);
    Event(MouseButtonPressedEvent event);
    Event(MouseButtonReleasedEvent event);
    Event(MouseMovedEvent event);
    Event(MouseWheelScrolledEvent event);

    [[nodiscard]] auto type() const { return _type; }
    [[nodiscard]] auto category() const { return _category; }

    [[nodiscard]] auto window_resized_event() const -> WindowResizedEvent;
    [[nodiscard]] auto key_pressed_event() const -> KeyPressedEvent;
    [[nodiscard]] auto key_released_event() const -> KeyReleasedEvent;
    [[nodiscard]] auto mouse_button_pressed_event() const -> MouseButtonPressedEvent;
    [[nodiscard]] auto mouse_button_released_event() const -> MouseButtonReleasedEvent;
    [[nodiscard]] auto mouse_moved_event() const -> MouseMovedEvent;
    [[nodiscard]] auto mouse_wheel_scrolled_event() const -> MouseWheelScrolledEvent;

private:
    EventType _type;
    EventCategory _category;

    union
    {
        WindowResizedEvent _window_resized_event;
        KeyPressedEvent _key_pressed_event;
        KeyReleasedEvent _key_released_event;
        MouseButtonPressedEvent _mouse_button_pressed_event;
        MouseButtonReleasedEvent _mouse_button_released_event;
        MouseMovedEvent _mouse_moved_event;
        MouseWheelScrolledEvent _mouse_wheel_scrolled_event;
    };
};

} // namespace zth
