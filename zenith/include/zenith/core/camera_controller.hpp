#pragma once

#include <glm/trigonometric.hpp>

#include <memory>

#include "zenith/graphics/camera.hpp"
#include "zenith/platform/input.hpp"

namespace zth {

class FpsCameraController
{
public:
    float movement_speed = 1.5f;
    float sensitivity = 0.001f;

    float min_pitch = glm::radians(-89.0f);
    float max_pitch = glm::radians(89.0f);

    Key move_forward_key = Key::W;
    Key move_back_key = Key::S;
    Key move_left_key = Key::A;
    Key move_right_key = Key::D;

    bool sprint_enabled = true;
    float sprint_speed_multiplier = 3.0f;
    Key sprint_key = Key::LeftShift;

public:
    explicit FpsCameraController(std::shared_ptr<PerspectiveCamera> camera);

    auto on_update() const -> void;

    auto set_camera(std::shared_ptr<PerspectiveCamera> camera) -> void;

    [[nodiscard]] auto camera() -> auto& { return _camera; }
    [[nodiscard]] auto camera() const -> auto& { return _camera; }

private:
    std::shared_ptr<PerspectiveCamera> _camera;
};

} // namespace zth
