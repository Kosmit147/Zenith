#pragma once

class Scene : public zth::Scene
{
public:
    explicit Scene();
    ZTH_NO_COPY_NO_MOVE(Scene)
    ~Scene() override = default;

private:
    zth::Entity _cube = create_entity("Cube");
    zth::Entity _light = create_entity("Light");
    zth::gl::Texture2D _cube_texture;
    zth::Material _cube_material;
    std::shared_ptr<zth::PerspectiveCamera> _camera;
    zth::FpsCameraController _camera_controller;

private:
    auto on_load() -> void override;
    auto on_update() -> void override;
    auto on_event(const zth::Event& event) -> void override;

    auto on_window_resized_event(const zth::WindowResizedEvent& event) const -> void;
};
