#include "cube_game.hpp"

#include "scene.hpp"

ZTH_IMPLEMENT_APP(CubeGame)

namespace {

const zth::ApplicationSpec app_spec = {
    .window_spec = {
        .size = { 800, 600 },
        .title = "Cube Game",
        .fullscreen = false,
        .vsync = true,
        .frame_rate_limit = 60,
        .maximized = true,
        .cursor_enabled = false,
    },
    .logger_spec = {
        .client_logger_label = "CUBE GAME",
        .log_file_path = "log/log.txt",
    }
};

} // namespace

CubeGame::CubeGame() : Application(app_spec)
{
    zth::SceneManager::load_scene(std::make_unique<Scene>());
}

auto CubeGame::on_event(const zth::Event& event) -> void
{
    if (event.type() == zth::EventType::KeyPressed)
        on_key_pressed_event(event.key_pressed_event());
}

auto CubeGame::on_update() -> void
{
    _debug_tools_panel.draw();
}

auto CubeGame::on_key_pressed_event(const zth::KeyPressedEvent& event) -> void
{
    _debug_tools_panel.on_key_pressed_event(event);

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
