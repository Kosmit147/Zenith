#pragma once

class MainScene : public zth::Scene
{
public:
    explicit MainScene();
    ZTH_NO_COPY_NO_MOVE(MainScene)
    ~MainScene() override = default;

private:
    zth::Entity _cube = create_entity("Cube");

    zth::Vector<zth::gl::Texture2D> _diffuse_maps;
    zth::Vector<zth::gl::Texture2D> _specular_maps;
    zth::Vector<zth::gl::Texture2D> _emission_maps;

    zth::Material _cube_material;
    zth::Material _point_light_1_material;
    zth::Material _point_light_2_material;
    zth::Material _point_light_3_material;

    std::shared_ptr<zth::PerspectiveCamera> _camera;
    zth::FpsCameraController _camera_controller;

    zth::Entity _directional_light = create_entity("Directional Light");

    zth::Entity _point_light_1 = create_entity("Point Light 1");
    zth::Entity _point_light_2 = create_entity("Point Light 2");
    zth::Entity _point_light_3 = create_entity("Point Light 3");

    zth::debug::MaterialPanel _material_panel{ _cube_material };

private:
    auto on_load() -> void override;
    auto on_update() -> void override;
    auto on_event(const zth::Event& event) -> void override;

    auto on_window_resized_event(const zth::WindowResizedEvent& event) -> void;

    auto update_ui() -> void;
};
