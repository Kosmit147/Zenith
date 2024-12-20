#include "Testbed.hpp"

#include <imgui.h>

#include "TransformTest.hpp"

ZTH_IMPLEMENT_APP(Testbed)

namespace {

const zth::ApplicationSpec app_spec = {
    .window_spec = {
        .size = { 800, 600 },
        .title = "Testbed",
        .fullscreen = false,
        .vsync = true,
        .maximized = true,
        .cursor_enabled = false,
    },
    .logger_spec = {
        .client_logger_label = "TESTBED",
        .log_file_path = "log/log.txt",
    }
};

} // namespace

Testbed::Testbed() : Application(app_spec)
{
    zth::SceneManager::load_scene(std::make_unique<TransformTest>());
    ImGui::GetIO().FontGlobalScale = 1.5f;
}

auto Testbed::on_update() -> void {}

auto Testbed::on_event(const zth::Event& event) -> void
{
    switch (event.type())
    {
        using enum zth::EventType;
    case WindowResized:
    {
        auto [new_size] = event.window_resized_event();
        ZTH_INFO("Window resized. New size: {}.", new_size);
        break;
    }
    case KeyPressed:
    {
        auto key_pressed_event = event.key_pressed_event();
        ZTH_INFO("{} key pressed.", key_pressed_event.key);
        on_key_pressed_event(key_pressed_event);
        break;
    }
    case KeyReleased:
    {
        auto [key] = event.key_released_event();
        ZTH_INFO("{} key released.", key);
        break;
    }
    case MouseButtonPressed:
    {
        auto [button] = event.mouse_button_pressed_event();
        ZTH_INFO("{} mouse button pressed.", button);
        break;
    }
    case MouseButtonReleased:
    {
        auto [button] = event.mouse_button_released_event();
        ZTH_INFO("{} mouse button released.", button);
        break;
    }
    case MouseMoved:
    {
        auto [new_pos] = event.mouse_moved_event();
        ZTH_INFO("Mouse moved. New pos: {}.", new_pos);
        break;
    }
    case MouseWheelScrolled:
    {
        auto [delta] = event.mouse_wheel_scrolled_event();
        ZTH_INFO("Mouse scrolled. Delta: {}.", delta);
        break;
    }
    }
}

auto Testbed::on_key_pressed_event(const zth::KeyPressedEvent& event) -> void
{
    static bool cursor_enabled = app_spec.window_spec.cursor_enabled;

    switch (event.key)
    {
        using enum zth::Key;
    case Escape:
        zth::Window::close();
        break;
    case LeftControl:
        cursor_enabled = !cursor_enabled;
        zth::Window::set_cursor_enabled(cursor_enabled);
        break;
    }
}
