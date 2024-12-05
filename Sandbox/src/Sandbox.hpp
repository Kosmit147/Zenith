#pragma once

#include <Zenith/Zenith.hpp>

#include <memory>

class Sandbox : public zth::Application
{
public:
    explicit Sandbox();
    ~Sandbox() override = default;

private:
    zth::CubeShape _cube;
    zth::Texture2D _texture;
    std::shared_ptr<zth::PerspectiveCamera> _camera;

private:
    auto on_update() -> void override;
    auto on_event(const zth::Event& event) -> void override;

    auto on_window_resized_event(const zth::WindowResizedEvent& event) const -> void;
    auto update_camera() const -> void;
};
