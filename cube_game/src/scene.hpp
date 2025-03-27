#pragma once

class Scene : public zth::Scene
{
public:
    explicit Scene();
    ZTH_NO_COPY_NO_MOVE(Scene)
    ~Scene() override = default;

private:
    zth::EntityHandle _camera = create_entity("Camera");
    zth::EntityHandle _directional_light = create_entity("Directional Light");
    zth::Vector<zth::EntityHandle> _blocks;

    zth::gl::Texture2D _block_texture;
    zth::Material _block_material;

private:
    auto on_load() -> void override;
    auto on_event(const zth::Event& event) -> void override;

    auto on_window_resized_event(const zth::WindowResizedEvent& event) const -> void;
};
