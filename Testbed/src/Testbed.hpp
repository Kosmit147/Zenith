#pragma once

#include <Zenith/Zenith.hpp>

class Testbed : public zth::Application
{
public:
    explicit Testbed();
    ZTH_NO_COPY_NO_MOVE(Testbed)
    ~Testbed() override = default;

private:
    zth::debug::DebugToolsUi _debug_tools_ui;

private:
    auto on_update() -> void override;
    auto on_event(const zth::Event& event) -> void override;

    auto on_key_pressed_event(const zth::KeyPressedEvent& event) -> void;

    auto update_ui() -> void;
};
