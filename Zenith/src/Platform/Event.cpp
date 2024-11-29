#include "Zenith/Platform/Event.hpp"

#include "Zenith/Core/Assert.hpp"

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

} // namespace zth
