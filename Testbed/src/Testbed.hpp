#pragma once

#include <Zenith/Zenith.hpp>

class Testbed : public zth::Application
{
public:
    static constexpr auto toggle_cursor_key = zth::Key::LeftControl;

public:
    explicit Testbed();
    ZTH_NO_COPY_NO_MOVE(Testbed)
    ~Testbed() override = default;

private:
    zth::debug::DebugToolsUi _debug_tools_ui;
    zth::debug::ScenePickerUi _scene_picker_ui;

private:
    auto on_update() -> void override;
    auto on_event(const zth::Event& event) -> void override;

    auto on_key_pressed_event(const zth::KeyPressedEvent& event) -> void;

    auto update_ui() -> void;
};
