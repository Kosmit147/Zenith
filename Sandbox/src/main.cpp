#include <Zenith/Zenith.hpp>

namespace {

const zth::WindowSpec window_spec = {
    .width = 1920,
    .height = 1080,
    .title = "Sandbox",
    .gl_version = { 4, 6 },
    .gl_profile = zth::GlProfile::Core,
    .vsync = true,
};

}

auto main() -> int
{
    zth::Window window(window_spec);
    window.make_context_current();

    while (!window.should_close())
    {
        window.swap_buffers();
        window.poll_events();
    }
}
