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
    std::vector<zth::CubeShape> _containers;
    zth::SphereShape _light_marker;

    zth::gl::Texture2D _diffuse_map;
    zth::gl::Texture2D _specular_map;

    zth::Material _container_material;
    zth::Material _light_marker_material;

    std::shared_ptr<zth::PerspectiveCamera> _camera;
    zth::FpsCameraController _camera_controller;
    std::shared_ptr<zth::DirectionalLight> _directional_light;
    std::shared_ptr<zth::PointLight> _point_light;
    std::shared_ptr<zth::SpotLight> _spot_light;
    bool _spot_light_on = true;

    zth::debug::DirectionalLightUi _directional_light_ui{ *_directional_light };
    zth::debug::PointLightUi _point_light_ui{ *_point_light };
    zth::debug::SpotLightUi _spot_light_ui{ *_spot_light };

private:
    auto on_load() -> void override;
    auto on_update() -> void override;
    auto on_event(const zth::Event& event) -> void override;

    auto on_window_resized_event(const zth::WindowResizedEvent& event) -> void;
    auto on_key_pressed_event(const zth::KeyPressedEvent& event) -> void;

    auto update_ui() -> void;
};
