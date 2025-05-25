#pragma once

class Character : public zth::Scene
{
public:
    explicit Character();
    ZTH_NO_COPY_NO_MOVE(Character)
    ~Character() override = default;

private:
    zth::EntityHandle _player = create_entity("Player");
    zth::EntityHandle _crosshair = create_entity("Crosshair");

    zth::EntityHandle _floor = create_entity("Floor");
    zth::EntityHandle _ball_spawner = create_entity("Ball Spawner");

    zth::EntityHandle _directional_light = create_entity("Directional Light");

private:
    auto on_load() -> void override;
};
