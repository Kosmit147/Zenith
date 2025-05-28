#pragma once

class MainLayer : public zth::Layer
{
public:
    static constexpr auto toggle_cursor_key = zth::Key::LeftControl;

public:
    auto on_event(const zth::Event& event) -> void override;

private:
    auto on_key_pressed_event(const zth::KeyPressedEvent& event) -> void;
};
