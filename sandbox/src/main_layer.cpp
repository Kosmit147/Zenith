#include "main_layer.hpp"

auto MainLayer::on_attach() -> zth::Result<zth::Success, zth::String>
{
    return zth::Success{};
}

auto MainLayer::on_event(const zth::Event& event) -> void
{
    if (event.type() == zth::EventType::KeyPressed)
        on_key_pressed_event(event.key_pressed_event());
}

auto MainLayer::on_key_pressed_event(const zth::KeyPressedEvent& event) -> void
{
    switch (event.key)
    {
        using enum zth::Key;
    case Escape:
        zth::Window::close();
        break;
    case toggle_cursor_key:
        zth::Window::set_cursor_enabled(!zth::Window::cursor_enabled());
        break;
    }
}
