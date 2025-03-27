#include "zenith/script/camera_controller.hpp"

#include "zenith/ecs/components.hpp"
#include "zenith/system/time.hpp"
#include "zenith/system/window.hpp"

namespace zth::scripts {

auto FlyCamera::display_name() const -> const char*
{
    return "Fly Camera";
}

auto FlyCamera::on_update(EntityHandle actor) -> void
{
    if (Window::cursor_enabled())
        return;

    auto& transform = actor.get<TransformComponent>();

    auto delta_time = Time::delta_time<float>();
    auto speed = movement_speed * delta_time;

    if (sprinting_enabled)
    {
        if (Input::is_key_pressed(sprint_key))
            speed *= sprinting_speed_multiplier;
    }

    auto forward = transform.forward();
    auto backward = -forward;
    auto right = transform.right();
    auto left = -right;

    if (Input::is_key_pressed(move_forward_key))
        transform.translate(forward * speed);

    if (Input::is_key_pressed(move_backward_key))
        transform.translate(backward * speed);

    if (Input::is_key_pressed(move_right_key))
        transform.translate(right * speed);

    if (Input::is_key_pressed(move_left_key))
        transform.translate(left * speed);

    auto mouse_delta = Input::mouse_pos_delta() / static_cast<float>(Window::size().y) * mouse_sensitivity;
    auto angles = transform.euler_angles();

    angles.pitch -= mouse_delta.y;
    angles.yaw -= mouse_delta.x;
    angles.roll = 0.0f;

    if (clamp_pitch)
        angles.pitch = std::clamp(angles.pitch, min_pitch, max_pitch);

    transform.set_rotation(angles);
}

} // namespace zth::scripts
