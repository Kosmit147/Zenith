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
    zth::Vector<zth::EntityHandle> _containers;

    zth::gl::Texture2D _diffuse_map;
    zth::gl::Texture2D _specular_map;

    zth::Material _container_material;
    zth::Material _point_light_material;

    zth::EntityHandle _directional_light = create_entity("Directional Light");
    zth::EntityHandle _point_light = create_entity("Point Light");
    zth::EntityHandle _ambient_light = create_entity("Ambient Light");

    bool _spot_light_on = true;

private:
    auto on_event(const zth::Event& event) -> void override;
    auto on_update() -> void override;

    auto on_window_resized_event(const zth::WindowResizedEvent& event) const -> void;
    auto on_key_pressed_event(const zth::KeyPressedEvent& event) -> void;
};
