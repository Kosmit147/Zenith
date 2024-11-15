#include <Zenith/Zenith.hpp>

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
};

}

auto main() -> int
{
    auto maybe_app = zth::Application::create(app_spec);

    if (!maybe_app)
        return -1;

    auto& app = maybe_app.value();
    app.run();
}
