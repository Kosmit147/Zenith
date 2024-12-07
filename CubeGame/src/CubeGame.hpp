#pragma once

#include <Zenith/Zenith.hpp>

#include <memory>

class CubeGame : public zth::Application
{
public:
    explicit CubeGame();
    ZTH_NO_COPY_NO_MOVE(CubeGame)
    ~CubeGame() override = default;

private:
    zth::CubeShape _cube;
    zth::Texture2D _cobble_texture;
    std::shared_ptr<zth::PerspectiveCamera> _camera;

private:
    auto on_update() -> void override;
    auto on_event(const zth::Event& event) -> void override;

    auto update_camera() const -> void;
};
