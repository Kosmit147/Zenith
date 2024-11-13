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
    auto maybe_window = zth::Window::create(window_spec);

    if (!maybe_window)
        return 0;

    auto& window = maybe_window.value();

    window.set_active();

    while (!window.should_close())
    {
        window.swap_buffers();
        window.poll_events();
    }
}
