#pragma once

class Testbed : public zth::Application
{
public:
    static constexpr auto toggle_cursor_key = zth::Key::LeftControl;

public:
    explicit Testbed();
    ZTH_NO_COPY_NO_MOVE(Testbed)
    ~Testbed() override = default;

private:
    // @todo: Move to debug layer.
    zth::debug::DebugToolsPanel _debug_tools_panel;
    zth::debug::ScenePicker _scene_picker;

private:
    auto on_update() -> void override;
    auto on_event(const zth::Event& event) -> void override;

    auto on_key_pressed_event(const zth::KeyPressedEvent& event) -> void;

    auto draw_ui() -> void;
};
