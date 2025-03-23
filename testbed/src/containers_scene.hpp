#pragma once

class ContainersScene : public zth::Scene
{
public:
    static constexpr auto toggle_spotlight_key = zth::Key::F;

public:
    explicit ContainersScene();
    ZTH_NO_COPY_NO_MOVE(ContainersScene)
    ~ContainersScene() override = default;

private:
    zth::Vector<zth::EntityHandle> _containers;

    zth::gl::Texture2D _diffuse_map;
    zth::gl::Texture2D _specular_map;

    zth::Material _container_material;
    zth::Material _point_light_material;

    std::shared_ptr<zth::PerspectiveCamera> _camera;
    zth::FpsCameraController _camera_controller;

    zth::EntityHandle _directional_light = create_entity("Directional Light");
    zth::EntityHandle _point_light = create_entity("Point Light");
    zth::EntityHandle _spot_light = create_entity("Spot Light");
    zth::EntityHandle _ambient_light = create_entity("Ambient Light");

    bool _spot_light_on = true;

private:
    auto on_load() -> void override;
    auto on_event(const zth::Event& event) -> void override;
    auto on_update() -> void override;

    auto on_window_resized_event(const zth::WindowResizedEvent& event) -> void;
    auto on_key_pressed_event(const zth::KeyPressedEvent& event) -> void;
};
