#pragma once

class Sandbox : public zth::Application
{
public:
    explicit Sandbox();
    ZTH_NO_COPY_NO_MOVE(Sandbox)
    ~Sandbox() override = default;

private:
    // @todo: Move to debug layer.
    zth::debug::DebugToolsPanel _debug_tools_panel;

private:
    auto on_event(const zth::Event& event) -> void override;
    auto on_update() -> void override;

    auto on_key_pressed_event(const zth::KeyPressedEvent& event) -> void;
};
