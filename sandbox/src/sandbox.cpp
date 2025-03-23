#include "sandbox.hpp"

#include "scene.hpp"

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
    zth::SceneManager::load_scene(std::make_unique<Scene>());
}

auto Sandbox::on_event(const zth::Event& event) -> void
{
    if (event.type() == zth::EventType::KeyPressed)
        on_key_pressed_event(event.key_pressed_event());
}

auto Sandbox::on_update() -> void
{
    _debug_tools_panel.draw();
}

auto Sandbox::on_key_pressed_event(const zth::KeyPressedEvent& event) -> void
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
