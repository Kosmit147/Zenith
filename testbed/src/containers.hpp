#pragma once

class Containers : public zth::Scene
{
public:
    explicit Containers();
    ZTH_NO_COPY_NO_MOVE(Containers)
    ~Containers() override = default;

private:
    zth::EntityHandle _camera = create_entity("Camera");

    zth::EntityHandle _directional_light = create_entity("Directional Light");
    zth::EntityHandle _point_light = create_entity("Point Light");
    zth::EntityHandle _ambient_light = create_entity("Ambient Light");

private:
    auto on_unload() -> void override;
};
