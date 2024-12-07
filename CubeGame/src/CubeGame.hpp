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
};
