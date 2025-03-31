#pragma once

class MainLayer : public zth::Layer
{
public:
    static constexpr auto toggle_cursor_key = zth::Key::LeftControl;
    static constexpr auto next_scene_key = zth::Key::Left;
    static constexpr auto prev_scene_key = zth::Key::Right;

public:
    auto on_attach() -> zth::Result<void, zth::String> override;
    auto on_event(const zth::Event& event) -> void override;
    auto on_update() -> void override;

private:
    zth::debug::ScenePicker _scene_picker;

private:
    auto on_key_pressed_event(const zth::KeyPressedEvent& event) -> void;
};
