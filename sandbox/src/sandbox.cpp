#include "sandbox.hpp"

#include "main_layer.hpp"
#include "main_scene.hpp"

ZTH_IMPLEMENT_APP(Sandbox)

namespace {

const zth::ApplicationSpec app_spec = {
    .window_spec = {
        .size = { 800, 600 },
        .title = "Sandbox",
        .gl_version = { 4, 6 },
        .gl_profile = zth::gl::Profile::Core,
        .fullscreen = false,
        .vsync = true,
        .frame_rate_limit = 60,
        .resizable = true,
        .maximized = true,
        .cursor_enabled = false,
        .transparent_framebuffer = false,
        .forced_aspect_ratio = zth::nil,
    },
    .logger_spec = {
        .client_logger_label = "SANDBOX",
        .log_file_path = "log/log.txt",
    }
};

} // namespace

Sandbox::Sandbox() : Application(app_spec)
{
    push_layer(zth::make_unique<MainLayer>());
    zth::SceneManager::queue_scene<MainScene>();
}
