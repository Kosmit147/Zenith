#pragma once

#include <Zenith/Zenith.hpp>

#include "Block.hpp"
#include "Player.hpp"

class Scene : public zth::Scene
{
public:
    explicit Scene();
    ZTH_NO_COPY_NO_MOVE(Scene)
    ~Scene() override = default;

private:
    zth::Texture2D _block_texture;
    zth::Material _block_material;

    Block _block;
    Player _player;

private:
    auto on_load() -> void override;
    auto on_event(const zth::Event& event) -> void override;
    auto on_update() -> void override;
};
