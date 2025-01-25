#include "zenith/platform/input.hpp"

#include "zenith/core/assert.hpp"
#include "zenith/logging/logger.hpp"
#include "zenith/platform/event.hpp"
#include "zenith/platform/window.hpp"

namespace zth {

auto Input::init() -> void
{
    for (auto key : key_enumerations)
        _key_states.insert({ key, false });

    for (auto button : mouse_button_enumerations)
        _mouse_button_states.insert({ button, false });

    // a solution for sudden movements caused by the first read of mouse position
    _mouse_pos = Window::mouse_pos();
    _prev_mouse_pos = Window::mouse_pos();

    ZTH_CORE_INFO("Input system initialized.");
}

auto Input::on_update() -> void
{
    _mouse_pos_delta = _mouse_pos - _prev_mouse_pos;
    _prev_mouse_pos = _mouse_pos;

    if (!_mouse_scroll_delta_updated)
        _mouse_scroll_delta = { 0.0f, 0.0f };

    _mouse_scroll_delta_updated = false;
}

auto Input::on_input_event(const Event& event) -> void
{
    ZTH_ASSERT(event.category() == EventCategory::InputEvent);

    switch (event.type())
    {
        using enum EventType;
    case KeyPressed:
    {
        auto [key] = event.key_pressed_event();
        set_key_pressed(key, true);
    }
    break;
    case KeyReleased:
    {
        auto [key] = event.key_released_event();
        set_key_pressed(key, false);
    }
    break;
    case MouseButtonPressed:
    {
        auto [button] = event.mouse_button_pressed_event();
        set_mouse_button_pressed(button, true);
    }
    break;
    case MouseButtonReleased:
    {
        auto [button] = event.mouse_button_released_event();
        set_mouse_button_pressed(button, false);
    }
    break;
    case MouseMoved:
    {
        auto [new_pos] = event.mouse_moved_event();
        set_mouse_pos(new_pos);
    }
    break;
    case MouseWheelScrolled:
    {
        auto [delta] = event.mouse_wheel_scrolled_event();
        set_mouse_scroll_delta(delta);
    }
    break;
    default:
    {
        ZTH_ASSERT(false);
        std::unreachable();
    }
    }
}

auto Input::shut_down() -> void
{
    ZTH_CORE_INFO("Input system shut down.");
}

auto Input::is_key_pressed(Key key) -> bool
{
    return _key_states.at(key);
}

auto Input::set_key_pressed(Key key, bool value) -> void
{
    _key_states.insert_or_assign(key, value);
}

auto Input::is_mouse_button_pressed(MouseButton button) -> bool
{
    return _mouse_button_states.at(button);
}

auto Input::set_mouse_button_pressed(MouseButton button, bool value) -> void
{
    _mouse_button_states.insert_or_assign(button, value);
}

auto Input::mouse_pos() -> glm::vec2
{
    return _mouse_pos;
}

auto Input::mouse_pos_delta() -> glm::vec2
{
    return _mouse_pos_delta;
}

auto Input::mouse_scroll_delta() -> glm::vec2
{
    return _mouse_scroll_delta;
}

auto Input::set_mouse_pos(glm::vec2 value) -> void
{
    _mouse_pos = value;
}

auto Input::set_mouse_scroll_delta(glm::vec2 value) -> void
{
    _mouse_scroll_delta = value;
    _mouse_scroll_delta_updated = true;
}

} // namespace zth
