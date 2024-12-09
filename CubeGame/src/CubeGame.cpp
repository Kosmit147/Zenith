#include "CubeGame.hpp"

#include "Scene.hpp"

#include <memory>

ZTH_IMPLEMENT_APP(CubeGame)

namespace {

const zth::ApplicationSpec app_spec = {
    .window_spec = {
        .size = { 800, 600 },
        .title = "Cube Game",
        .fullscreen = false,
        .vsync = false,
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

auto CubeGame::on_update() -> void {}

auto CubeGame::on_event(const zth::Event& event) -> void
{
    if (event.type() == zth::EventType::KeyPressed)
        on_key_pressed_event(event.key_pressed_event());
}

auto CubeGame::on_key_pressed_event(const zth::KeyPressedEvent& event) -> void
{
    if (event.key == zth::Key::Escape)
        zth::Window::close();
}
