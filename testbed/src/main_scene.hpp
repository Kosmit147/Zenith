#pragma once

class MainScene : public zth::Scene
{
public:
    explicit MainScene();
    ZTH_NO_COPY_NO_MOVE(MainScene)
    ~MainScene() override = default;

private:
    zth::CubeShape _cube;
    zth::SphereShape _light_marker;

    std::vector<zth::Texture2D> _diffuse_maps;
    std::vector<zth::Texture2D> _specular_maps;
    std::vector<zth::Texture2D> _emission_maps;

    zth::Material _material;
    zth::Material _light_marker_material;

    std::shared_ptr<zth::PerspectiveCamera> _camera;
    zth::FpsCameraController _camera_controller;
    std::shared_ptr<zth::DirectionalLight> _directional_light;
    std::shared_ptr<zth::PointLight> _point_light;

    zth::debug::TransformUi _transform_ui{ _cube };
    zth::debug::TransformGizmo _transform_gizmo{ _cube, *_camera };
    zth::debug::MaterialUi _material_ui{ _material };
    zth::debug::DirectionalLightUi _directional_light_ui{ *_directional_light };
    zth::debug::PointLightUi _point_light_ui{ *_point_light };

private:
    auto on_load() -> void override;
    auto on_update() -> void override;
    auto on_event(const zth::Event& event) -> void override;

    auto on_window_resized_event(const zth::WindowResizedEvent& event) -> void;
    auto on_key_pressed_event(const zth::KeyPressedEvent& event) -> void;

    auto update_ui() -> void;
};
