#pragma once

class Player
{
public:
    explicit Player();

    auto on_update() -> void;
    auto on_event(const zth::Event& event) -> void;

    [[nodiscard]] auto camera() const -> auto& { return _camera; }

private:
    std::shared_ptr<zth::PerspectiveCamera> _camera;
    zth::FpsCameraController _camera_controller;

private:
    auto on_window_resized_event(const zth::WindowResizedEvent& event) const -> void;
};
