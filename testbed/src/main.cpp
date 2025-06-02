#include "main_layer.hpp"
#include "main_scene.hpp"

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

auto main() -> int
{
    auto init_application_result = zth::Application::init(app_spec);

    if (!init_application_result)
    {
        std::println(std::cerr, "CRITICAL ERROR: {}", init_application_result.error());
        return -1;
    }

    auto push_layer_result = zth::Application::push_layer(zth::make_unique<MainLayer>());

    if (!push_layer_result)
    {
        std::println(std::cerr, "CRITICAL ERROR: {}", push_layer_result.error());
        return -1;
    }

    push_layer_result = zth::Application::push_overlay(zth::make_unique<zth::DebugOverlay>());

    if (!push_layer_result)
    {
        std::println(std::cerr, "CRITICAL ERROR: {}", push_layer_result.error());
        return -1;
    }

    zth::SceneManager::queue_scene<MainScene>();
    zth::Application::run();
}
