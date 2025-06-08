#include "zenith/script/camera.hpp"

#include "zenith/debug/ui.hpp"
#include "zenith/ecs/components.hpp"
#include "zenith/ecs/ecs.hpp"
#include "zenith/system/application.hpp"
#include "zenith/system/event.hpp"
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

    debug::select_enum("Move Forward Key", move_forward_key);
    debug::select_enum("Move Backward Key", move_backward_key);
    debug::select_enum("Move Left Key", move_left_key);
    debug::select_enum("Move Right Key", move_right_key);

    debug::checkbox("Sprinting Enabled", sprinting_enabled);

    if (sprinting_enabled)
    {
        debug::select_enum("Sprint Key", sprint_key);
        debug::drag_float("Sprinting Speed Multiplier", sprinting_speed_multiplier);
    }
}

auto FlyCamera::on_event(EntityHandle actor, const Event& event) -> void
{
    if (event.type() == EventType::WindowResized)
    {
        auto [new_size] = event.window_resized_event();

        if (auto camera = actor.try_get<CameraComponent>())
            camera->get().aspect_ratio = static_cast<float>(new_size.x) / static_cast<float>(new_size.y);
    }
}

auto FlyCamera::on_update(EntityHandle actor) -> void
{
    auto& transform = actor.transform();

    {
        // Move around.

        auto speed = movement_speed * static_cast<float>(Application::delta_time());

        if (sprinting_enabled && Input::is_key_pressed(sprint_key))
            speed *= sprinting_speed_multiplier;

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

auto FlyCamera::on_attach(EntityHandle actor) -> void
{
    actor.try_emplace<CameraComponent>();
}

} // namespace zth::scripts
