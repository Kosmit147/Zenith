#pragma once

class Containers : public zth::Scene
{
public:
    static constexpr auto toggle_spotlight_key = zth::Key::F;

public:
    explicit Containers();
    ZTH_NO_COPY_NO_MOVE(Containers)
    ~Containers() override = default;

private:
    zth::EntityHandle _camera = create_entity("Camera");

    zth::EntityHandle _directional_light = create_entity("Directional Light");
    zth::EntityHandle _point_light = create_entity("Point Light");
    zth::EntityHandle _ambient_light = create_entity("Ambient Light");

    bool _spot_light_on = true;

private:
    auto on_event(const zth::Event& event) -> void override;
    auto on_update() -> void override;
    auto on_unload() -> void override;

    auto on_key_pressed_event(const zth::KeyPressedEvent& event) -> void;
};
