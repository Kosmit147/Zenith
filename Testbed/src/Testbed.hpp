#pragma once

#include <Zenith/Zenith.hpp>

#include <array>
#include <string_view>

class Testbed : public zth::Application
{
public:
    static constexpr std::size_t scene_count = 2;
    static constexpr std::array<std::string_view, scene_count> scene_names = { "Main Scene", "Containers" };

    static constexpr auto toggle_cursor_key = zth::Key::LeftControl;
    static constexpr auto prev_scene_key = zth::Key::Left;
    static constexpr auto next_scene_key = zth::Key::Right;

public:
    explicit Testbed();
    ZTH_NO_COPY_NO_MOVE(Testbed)
    ~Testbed() override = default;

private:
    std::size_t _scene_idx = 0;
    zth::debug::DebugToolsUi _debug_tools_ui;

private:
    auto on_update() -> void override;
    auto on_event(const zth::Event& event) -> void override;

    auto on_key_pressed_event(const zth::KeyPressedEvent& event) -> void;

    auto update_ui() -> void;
    auto draw_scene_picker_ui() -> void;

    auto prev_scene() -> void;
    auto next_scene() -> void;
    auto load_scene(std::size_t scene_idx) -> void;
};
