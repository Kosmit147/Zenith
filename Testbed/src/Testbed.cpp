#include "Testbed.hpp"

#include <imgui.h>

#include "ContainersScene.hpp"
#include "MainScene.hpp"

ZTH_IMPLEMENT_APP(Testbed)

namespace {

const zth::ApplicationSpec app_spec = {
    .window_spec = {
        .size = { 800, 600 },
        .title = "Testbed",
        .fullscreen = false,
        .vsync = true,
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
    zth::SceneManager::load_scene(std::make_unique<MainScene>());
    ImGui::GetIO().FontGlobalScale = 1.5f;
}

auto Testbed::on_update() -> void
{
    update_ui();
}

auto Testbed::on_event(const zth::Event& event) -> void
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
        auto key_pressed_event = event.key_pressed_event();
        ZTH_INFO("{} key pressed.", key_pressed_event.key);
        on_key_pressed_event(key_pressed_event);
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

auto Testbed::on_key_pressed_event(const zth::KeyPressedEvent& event) -> void
{
    _debug_tools_ui.on_key_pressed_event(event);

    static bool cursor_enabled = app_spec.window_spec.cursor_enabled;

    switch (event.key)
    {
        using enum zth::Key;
    case Escape:
        zth::Window::close();
        break;
    case toggle_cursor_key:
        cursor_enabled = !cursor_enabled;
        zth::Window::set_cursor_enabled(cursor_enabled);
        break;
    case prev_scene_key:
        prev_scene();
        break;
    case next_scene_key:
        next_scene();
        break;
    }
}

auto Testbed::update_ui() -> void
{
    _debug_tools_ui.on_update();
    draw_scene_picker_ui();
}

auto Testbed::draw_scene_picker_ui() -> void
{
    ImGui::Begin("Scene");

    ImGui::Text("%s", scene_names[_scene_idx].data());

    auto prev_scene_label = fmt::format("Prev ({})", prev_scene_key);
    ImGui::TextUnformatted(prev_scene_label.c_str());

    ImGui::SameLine();

    if (ImGui::ArrowButton("Prev", ImGuiDir_Left))
        prev_scene();

    ImGui::SameLine();

    if (ImGui::ArrowButton("Next", ImGuiDir_Right))
        next_scene();

    ImGui::SameLine();

    auto next_scene_label = fmt::format("Next ({})", next_scene_key);
    ImGui::TextUnformatted(next_scene_label.c_str());

    ImGui::End();
}

auto Testbed::prev_scene() -> void
{
    _scene_idx = (_scene_idx - 1) % scene_count;
    load_scene(_scene_idx);
}

auto Testbed::next_scene() -> void
{
    _scene_idx = (_scene_idx + 1) % scene_count;
    load_scene(_scene_idx);
}

auto Testbed::load_scene(std::size_t scene_idx) -> void
{
    static_assert(scene_count == 2);

    switch (scene_idx)
    {
    case 0:
        zth::SceneManager::load_scene(std::make_unique<MainScene>());
        break;
    case 1:
        zth::SceneManager::load_scene(std::make_unique<ContainersScene>());
        break;
    default:
        ZTH_ASSERT(false);
        break;
    }
}
