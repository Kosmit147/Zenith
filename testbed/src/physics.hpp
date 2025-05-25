#pragma once

class Physics : public zth::Scene
{
public:
    explicit Physics();
    ZTH_NO_COPY_NO_MOVE(Physics)
    ~Physics() override = default;

private:
    zth::EntityHandle _camera = create_entity("Camera");
    zth::EntityHandle _crosshair = create_entity("Crosshair");

    zth::EntityHandle _floor = create_entity("Floor");
    zth::EntityHandle _test_mesh = create_entity("Test Mesh");
    zth::EntityHandle _ball_spawner = create_entity("Ball Spawner");

    zth::EntityHandle _directional_light = create_entity("Directional Light");

private:
    auto on_load() -> void override;
};
