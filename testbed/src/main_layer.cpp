#include "main_layer.hpp"

#include "containers.hpp"
#include "main_scene.hpp"
#include "sprites.hpp"

auto MainLayer::on_attach() -> zth::Result<void, zth::String>
{
    _scene_picker.add_scene<MainScene>("Main Scene");
    _scene_picker.add_scene<Containers>("Containers");
    _scene_picker.add_scene<Sprites>("Sprites");
    return {};
}

auto MainLayer::on_event(const zth::Event& event) -> void
{
    if (event.type() == zth::EventType::KeyPressed)
        on_key_pressed_event(event.key_pressed_event());
}

auto MainLayer::on_update() -> void
{
    _scene_picker.display();
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
    case next_scene_key:
        _scene_picker.next_scene();
        break;
    case prev_scene_key:
        _scene_picker.prev_scene();
        break;
    }
}
