#include "testbed.hpp"

#include "containers_scene.hpp"
#include "main_scene.hpp"

ZTH_IMPLEMENT_APP(Testbed)

namespace {

const zth::ApplicationSpec app_spec = {
    .window_spec = {
        .size = { 800, 600 },
        .title = "Testbed",
        .fullscreen = false,
        .vsync = false,
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
    zth::SceneManager::load_scene(std::make_unique<MainScene>());
    _scene_picker_ui.add_scene<MainScene>("Main Scene");
    _scene_picker_ui.add_scene<ContainersScene>("Containers");
}

auto Testbed::on_update() -> void
{
    update_ui();
}

auto Testbed::on_event(const zth::Event& event) -> void
{
    // log_event(event);

    if (event.type() == zth::EventType::KeyPressed)
        on_key_pressed_event(event.key_pressed_event());
}

auto Testbed::on_key_pressed_event(const zth::KeyPressedEvent& event) -> void
{
    _debug_tools_ui.on_key_pressed_event(event);
    _scene_picker_ui.on_key_pressed_event(event);

    static bool cursor_enabled = app_spec.window_spec.cursor_enabled;

    switch (event.key)
    {
        using enum zth::Key;
    case Escape:
        zth::Window::close();
        break;
    case toggle_cursor_key:
        cursor_enabled = !cursor_enabled;
        zth::Window::set_cursor_enabled(cursor_enabled);
        break;
    }
}

auto Testbed::log_event(const zth::Event& event) -> void
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
        auto [key] = event.key_pressed_event();
        ZTH_INFO("{} key pressed.", key);
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

auto Testbed::update_ui() -> void
{
    _debug_tools_ui.on_update();
    _scene_picker_ui.on_update();
}
