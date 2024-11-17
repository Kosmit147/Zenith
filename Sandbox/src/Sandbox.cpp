#include "Sandbox.hpp"

namespace {

const zth::ApplicationSpec app_spec = {
    .window_spec = {
        .width = 1920,
        .height = 1080,
        .title = "Sandbox",
        .gl_version = { 4, 6 },
        .gl_profile = zth::GlProfile::Core,
        .vsync = true,
    },
    .logger_spec = {
        .client_logger_label = "SANDBOX",
        .log_file_path = "log/log.txt",
    }
};

}

ZTH_IMPLEMENT_APP(Sandbox)

Sandbox::Sandbox() : Application(app_spec) {}

auto Sandbox::on_update() -> void
{
    ZTH_INFO("On update!");
}
