#pragma once

#include <Zenith/Zenith.hpp>

class Scene : public zth::Scene
{
public:
    explicit Scene();
    ZTH_NO_COPY_NO_MOVE(Scene)
    ~Scene() override = default;
    
private:
    zth::CubeShape _cube;
    zth::Texture2D _cobble_texture;
    zth::Material _cobble_material;
    std::shared_ptr<zth::PerspectiveCamera> _camera;

private:
    auto on_load() -> void override;
    auto on_event(const zth::Event& event) -> void override;
    auto on_update() -> void override;
    auto on_window_resized_event(const zth::WindowResizedEvent& event) const -> void;

    auto update_camera() const -> void;
};