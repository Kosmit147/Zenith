#include "CubeGame.hpp"

#include "Scene.hpp"

ZTH_IMPLEMENT_APP(CubeGame)

namespace {

const zth::ApplicationSpec app_spec = {
    .window_spec = {
        .size = { 800, 600 },
        .title = "Cube Game",
        .vsync = true,
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

auto CubeGame::on_update() -> void
{
    if (zth::Input::is_key_pressed(zth::Key::Escape))
        zth::Window::close();
}
