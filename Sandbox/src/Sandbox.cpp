#include "Sandbox.hpp"

#include "Scene.hpp"

ZTH_IMPLEMENT_APP(Sandbox)

namespace {

const zth::ApplicationSpec app_spec = {
    .window_spec = {
        .size = { 800, 600 },
        .title = "Sandbox",
        .gl_version = { 4, 6 },
        .gl_profile = zth::GlProfile::Core,
        .fullscreen = false,
        .vsync = true,
        .resizable = true,
        .maximized = true,
        .cursor_enabled = false,
        .transparent_framebuffer = false,
        .forced_aspect_ratio = std::nullopt,
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

auto Sandbox::on_update() -> void
{
    if (zth::Input::is_key_pressed(zth::Key::Escape))
        zth::Window::close();
}

auto Sandbox::on_event(const zth::Event& event) -> void
{
    switch (event.type())
    {
        using enum zth::EventType;
    case WindowResized:
    {
        auto [new_size] = event.window_resized_event();
        ZTH_INFO("Window resized. New size: {}.", new_size);
        break;
    }
    case KeyPressed:
    {
        auto [key] = event.key_pressed_event();
        ZTH_INFO("{} key pressed.", key);
        break;
    }
    case KeyReleased:
    {
        auto [key] = event.key_released_event();
        ZTH_INFO("{} key released.", key);
        break;
    }
    case MouseButtonPressed:
    {
        auto [button] = event.mouse_button_pressed_event();
        ZTH_INFO("{} mouse button pressed.", button);
        break;
    }
    case MouseButtonReleased:
    {
        auto [button] = event.mouse_button_released_event();
        ZTH_INFO("{} mouse button released.", button);
        break;
    }
    case MouseMoved:
    {
        auto [new_pos] = event.mouse_moved_event();
        ZTH_INFO("Mouse moved. New pos: {}.", new_pos);
        break;
    }
    case MouseWheelScrolled:
    {
        auto [delta] = event.mouse_wheel_scrolled_event();
        ZTH_INFO("Mouse scrolled. Delta: {}.", delta);
        break;
    }
    }
}
