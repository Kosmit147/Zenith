#pragma once

class Scene : public zth::Scene
{
public:
    explicit Scene();
    ZTH_NO_COPY_NO_MOVE(Scene)
    ~Scene() override = default;

private:
    zth::EntityHandle _camera = create_entity("Camera");
    zth::EntityHandle _cube = create_entity("Cube");
    zth::EntityHandle _light = create_entity("Light");
    zth::gl::Texture2D _cube_texture;
    zth::Material _cube_material;

private:
    auto on_event(const zth::Event& event) -> void override;

    auto on_window_resized_event(const zth::WindowResizedEvent& event) const -> void;
};
