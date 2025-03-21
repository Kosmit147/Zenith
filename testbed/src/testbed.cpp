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
        .frame_rate_limit = 60,
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
    _scene_picker.add_scene<MainScene>("Main Scene");
    _scene_picker.add_scene<ContainersScene>("Containers");
}

auto Testbed::on_update() -> void
{
    draw_ui();
}

auto Testbed::on_event(const zth::Event& event) -> void
{
    if (event.type() == zth::EventType::KeyPressed)
        on_key_pressed_event(event.key_pressed_event());
}

auto Testbed::on_key_pressed_event(const zth::KeyPressedEvent& event) -> void
{
    _debug_tools_panel.on_key_pressed_event(event);
    _scene_picker.on_key_pressed_event(event);

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

auto Testbed::draw_ui() -> void
{
    _debug_tools_panel.draw();
    _scene_picker.draw();
}
