#pragma once

class Sprites : public zth::Scene
{
public:
    explicit Sprites();
    ZTH_NO_COPY_NO_MOVE(Sprites)
    ~Sprites() override = default;

private:
    zth::EntityHandle _camera = create_entity("Camera");

private:
    auto on_load() -> void override;
    auto on_unload() -> void override;
};
