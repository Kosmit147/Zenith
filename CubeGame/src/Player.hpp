#pragma once

#include <Zenith/Zenith.hpp>

class Player : public zth::Updatable, public zth::EventListener
{
public:
    explicit Player();

    auto on_update() -> void override;
    auto on_event(const zth::Event& event) -> void override;

    [[nodiscard]] auto camera() const -> auto& { return _camera; }
    
private:
    std::shared_ptr<zth::PerspectiveCamera> _camera;

private:
    auto on_window_resized_event(const zth::WindowResizedEvent& event) const -> void;

    auto update_camera() const -> void;
};