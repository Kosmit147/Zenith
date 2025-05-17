#pragma once

namespace scripts {

class Camera : public zth::Script
{
public:
    float movement_speed = 1.5f;
    float mouse_sensitivity = 1.4f;

    bool clamp_pitch = true;
    float min_pitch = glm::radians(-89.0f);
    float max_pitch = glm::radians(89.0f);

    zth::Key move_forward_key = zth::Key::W;
    zth::Key move_backward_key = zth::Key::S;
    zth::Key move_left_key = zth::Key::A;
    zth::Key move_right_key = zth::Key::D;

    bool sprinting_enabled = true;
    zth::Key sprint_key = zth::Key::LeftShift;
    float sprinting_speed_multiplier = 3.0f;

    zth::Key toggle_flashlight_key = zth::Key::F;

public:
    Camera() = default;
    ZTH_DEFAULT_COPY_DEFAULT_MOVE(Camera)
    ~Camera() override = default;

    [[nodiscard]] auto display_label() const -> const char* override;
    auto debug_edit() -> void override;

    auto on_event(zth::EntityHandle actor, const zth::Event& event) -> void override;
    auto on_update(zth::EntityHandle actor) -> void override;

private:
    bool _flashlight_on = false;

private:
    auto on_attach(zth::EntityHandle actor) -> void override;

    auto on_window_resized_event(zth::EntityHandle actor, const zth::WindowResizedEvent& event) -> void;
    auto on_key_pressed_event(zth::EntityHandle actor, const zth::KeyPressedEvent& event) -> void;
};

} // namespace scripts
