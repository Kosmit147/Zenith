#include "testbed.hpp"

#include "main_layer.hpp"
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
    (void)push_layer(zth::make_unique<MainLayer>());
    zth::SceneManager::queue_scene<MainScene>();
}
