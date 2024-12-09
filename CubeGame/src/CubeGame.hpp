#pragma once

#include <Zenith/Zenith.hpp>

class CubeGame : public zth::Application
{
public:
    explicit CubeGame();
    ZTH_NO_COPY_NO_MOVE(CubeGame)
    ~CubeGame() override = default;

private:
    auto on_update() -> void override;
    auto on_event(const zth::Event& event) -> void override;

    static auto on_key_pressed_event(const zth::KeyPressedEvent& event) -> void;
};
