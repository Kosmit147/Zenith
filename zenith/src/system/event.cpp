#include "zenith/system/event.hpp"

#include "zenith/core/assert.hpp"
#include "zenith/log/formatters.hpp"

namespace zth {

Event::Event(WindowResizedEvent event)
    : _type(EventType::WindowResized), _category(EventCategory::WindowEvent), _window_resized_event(event)
{}

Event::Event(KeyPressedEvent event)
    : _type(EventType::KeyPressed), _category(EventCategory::InputEvent), _key_pressed_event(event)
{}

Event::Event(KeyReleasedEvent event)
    : _type(EventType::KeyReleased), _category(EventCategory::InputEvent), _key_released_event(event)
{}

Event::Event(MouseButtonPressedEvent event)
    : _type(EventType::MouseButtonPressed), _category(EventCategory::InputEvent), _mouse_button_pressed_event(event)
{}

Event::Event(MouseButtonReleasedEvent event)
    : _type(EventType::MouseButtonReleased), _category(EventCategory::InputEvent), _mouse_button_released_event(event)
{}

Event::Event(MouseMovedEvent event)
    : _type(EventType::MouseMoved), _category(EventCategory::InputEvent), _mouse_moved_event(event)
{}

Event::Event(MouseWheelScrolledEvent event)
    : _type(EventType::MouseWheelScrolled), _category(EventCategory::InputEvent), _mouse_wheel_scrolled_event(event)
{}

auto Event::window_resized_event() const -> WindowResizedEvent
{
    ZTH_ASSERT(_type == EventType::WindowResized);
    return _window_resized_event;
}

auto Event::key_pressed_event() const -> KeyPressedEvent
{
    ZTH_ASSERT(_type == EventType::KeyPressed);
    return _key_pressed_event;
}

auto Event::key_released_event() const -> KeyReleasedEvent
{
    ZTH_ASSERT(_type == EventType::KeyReleased);
    return _key_released_event;
}

auto Event::mouse_button_pressed_event() const -> MouseButtonPressedEvent
{
    ZTH_ASSERT(_type == EventType::MouseButtonPressed);
    return _mouse_button_pressed_event;
}

auto Event::mouse_button_released_event() const -> MouseButtonReleasedEvent
{
    ZTH_ASSERT(_type == EventType::MouseButtonReleased);
    return _mouse_button_released_event;
}

auto Event::mouse_moved_event() const -> MouseMovedEvent
{
    ZTH_ASSERT(_type == EventType::MouseMoved);
    return _mouse_moved_event;
}

auto Event::mouse_wheel_scrolled_event() const -> MouseWheelScrolledEvent
{
    ZTH_ASSERT(_type == EventType::MouseWheelScrolled);
    return _mouse_wheel_scrolled_event;
}

auto to_string(EventType event_type) -> const char*
{
    switch (event_type)
    {
        using enum EventType;
    case WindowResized:
        return "WindowResized";
    case KeyPressed:
        return "KeyPressed";
    case KeyReleased:
        return "KeyReleased";
    case MouseButtonPressed:
        return "MouseButtonPressed";
    case MouseButtonReleased:
        return "MouseButtonReleased";
    case MouseMoved:
        return "MouseMoved";
    case MouseWheelScrolled:
        return "MouseWheelScrolled";
    }

    ZTH_ASSERT(false);
    return "Unknown";
}

auto to_string(EventCategory event_category) -> const char*
{
    switch (event_category)
    {
        using enum EventCategory;
    case WindowEvent:
        return "WindowEvent";
    case InputEvent:
        return "InputEvent";
    }

    ZTH_ASSERT(false);
    return "Unknown";
}

} // namespace zth

ZTH_DEFINE_FORMATTER(zth::EventType, event_type)
{
    return ZTH_FORMAT_OUT("{}", zth::to_string(event_type));
}

ZTH_DEFINE_FORMATTER(zth::EventCategory, event_category)
{
    return ZTH_FORMAT_OUT("{}", zth::to_string(event_category));
}

ZTH_DEFINE_FORMATTER(zth::WindowResizedEvent, event)
{
    return ZTH_FORMAT_OUT("WindowResizedEvent {{\n"
                          "\t.new_size = {},\n"
                          "}}",
                          event.new_size);
}

ZTH_DEFINE_FORMATTER(zth::KeyPressedEvent, event)
{
    return ZTH_FORMAT_OUT("KeyPressedEvent {{\n"
                          "\t.key = {},\n"
                          "}}",
                          event.key);
}

ZTH_DEFINE_FORMATTER(zth::KeyReleasedEvent, event)
{
    return ZTH_FORMAT_OUT("KeyReleasedEvent {{\n"
                          "\t.key = {},\n"
                          "}}",
                          event.key);
}

ZTH_DEFINE_FORMATTER(zth::MouseButtonPressedEvent, event)
{
    return ZTH_FORMAT_OUT("MouseButtonPressedEvent {{\n"
                          "\t.button = {},\n"
                          "}}",
                          event.button);
}

ZTH_DEFINE_FORMATTER(zth::MouseButtonReleasedEvent, event)
{
    return ZTH_FORMAT_OUT("MouseButtonReleasedEvent {{\n"
                          "\t.button = {},\n"
                          "}}",
                          event.button);
}

ZTH_DEFINE_FORMATTER(zth::MouseMovedEvent, event)
{
    return ZTH_FORMAT_OUT("MouseMovedEvent {{\n"
                          "\t.new_pos = {},\n"
                          "}}",
                          event.new_pos);
}

ZTH_DEFINE_FORMATTER(zth::MouseWheelScrolledEvent, event)
{
    return ZTH_FORMAT_OUT("MouseWheelScrolledEvent {{\n"
                          "\t.delta = {},\n"
                          "}}",
                          event.delta);
}

ZTH_DEFINE_FORMATTER(zth::Event, event)
{
    ZTH_FORMAT_OUT("Event {{\n"
                   "\t.type = {},\n"
                   "\t.category = {},\n",
                   event.type(), event.category());

    switch (event.type())
    {
        using enum zth::EventType;
    case WindowResized:
        ZTH_FORMAT_OUT("\t.window_resized_event = {},\n", event.window_resized_event());
        break;
    case KeyPressed:
        ZTH_FORMAT_OUT("\t.key_pressed_event = {},\n", event.key_pressed_event());
        break;
    case KeyReleased:
        ZTH_FORMAT_OUT("\t.key_released_event = {},\n", event.key_released_event());
        break;
    case MouseButtonPressed:
        ZTH_FORMAT_OUT("\t.mouse_button_pressed_event = {},\n", event.mouse_button_pressed_event());
        break;
    case MouseButtonReleased:
        ZTH_FORMAT_OUT("\t.mouse_button_released_event = {},\n", event.mouse_button_released_event());
        break;
    case MouseMoved:
        ZTH_FORMAT_OUT("\t.mouse_moved_event = {},\n", event.mouse_moved_event());
        break;
    case MouseWheelScrolled:
        ZTH_FORMAT_OUT("\t.mouse_wheel_scrolled_event = {},\n", event.mouse_wheel_scrolled_event());
        break;
    }

    return ZTH_FORMAT_OUT("}}");
}
