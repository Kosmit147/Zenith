#pragma once

#include <glm/trigonometric.hpp>

#include "zenith/script/script.hpp"
#include "zenith/system/input.hpp"
#include "zenith/util/macros.hpp"

namespace zth::scripts {

class FlyCamera : public Script
{
public:
    float movement_speed = 1.5f;
    float mouse_sensitivity = 1.4f;

    bool clamp_pitch = true;
    float min_pitch = glm::radians(-89.0f);
    float max_pitch = glm::radians(89.0f);

    Key move_forward_key = Key::W;
    Key move_backward_key = Key::S;
    Key move_left_key = Key::A;
    Key move_right_key = Key::D;

    bool sprinting_enabled = true;
    Key sprint_key = Key::LeftShift;
    float sprinting_speed_multiplier = 3.0f;

public:
    FlyCamera() = default;
    ZTH_DEFAULT_COPY_DEFAULT_MOVE(FlyCamera)
    ~FlyCamera() override = default;

    [[nodiscard]] auto display_label() const -> const char* override;
    auto debug_edit() -> void override;

private:
    auto on_update(EntityHandle actor) -> void override;
};

} // namespace zth::scripts
