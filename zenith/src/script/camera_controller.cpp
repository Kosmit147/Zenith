#include "zenith/script/camera_controller.hpp"

#include "zenith/debug/ui.hpp"
#include "zenith/ecs/components.hpp"
#include "zenith/system/time.hpp"
#include "zenith/system/window.hpp"

namespace zth::scripts {

auto FlyCamera::display_label() const -> const char*
{
    return "Fly Camera";
}

auto FlyCamera::debug_edit() -> void
{
    debug::drag_float("Movement Speed", movement_speed);
    debug::drag_float("Mouse Sensitivity", mouse_sensitivity);

    debug::checkbox("Clamp Pitch", clamp_pitch);

    if (clamp_pitch)
    {
        debug::slide_angle("Minimum Pitch", min_pitch, -90.0f, 0.0f);
        debug::slide_angle("Maximum Pitch", max_pitch, 0.0f, 90.0f);
    }

    debug::select_key("Move Forward Key", move_forward_key);
    debug::select_key("Move Backward Key", move_backward_key);
    debug::select_key("Move Left Key", move_left_key);
    debug::select_key("Move Right Key", move_right_key);

    debug::checkbox("Sprinting Enabled", sprinting_enabled);

    if (sprinting_enabled)
    {
        debug::select_key("Sprint Key", sprint_key);
        debug::drag_float("Sprinting Speed Multiplier", sprinting_speed_multiplier);
    }
}

auto FlyCamera::on_update(EntityHandle actor) -> void
{
    auto& transform = actor.get<TransformComponent>();

    {
        // Move around.

        auto speed = movement_speed * Time::delta_time<float>();

        if (sprinting_enabled)
        {
            if (Input::is_key_pressed(sprint_key))
                speed *= sprinting_speed_multiplier;
        }

        auto forward = transform.forward() * speed;
        auto backward = -forward;
        auto right = transform.right() * speed;
        auto left = -right;

        if (Input::is_key_pressed(move_forward_key))
            transform.translate(forward);

        if (Input::is_key_pressed(move_backward_key))
            transform.translate(backward);

        if (Input::is_key_pressed(move_right_key))
            transform.translate(right);

        if (Input::is_key_pressed(move_left_key))
            transform.translate(left);
    }

    if (!Window::cursor_enabled())
    {
        // Look around.

        auto mouse_delta = Input::mouse_pos_delta() / static_cast<float>(Window::size().y) * mouse_sensitivity;
        auto angles = transform.euler_angles();

        angles.pitch -= mouse_delta.y;
        angles.yaw -= mouse_delta.x;
        angles.roll = 0.0f;

        if (clamp_pitch)
            angles.pitch = std::clamp(angles.pitch, min_pitch, max_pitch);

        transform.set_rotation(angles);
    }
}

} // namespace zth::scripts
