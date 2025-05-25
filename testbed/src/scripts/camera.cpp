#include "camera.hpp"

#include <glm/gtx/structured_bindings.hpp>

namespace scripts {

auto Camera::display_label() const -> const char*
{
    return "Camera";
}

auto Camera::debug_edit() -> void
{
    zth::debug::drag_float("Movement Speed", movement_speed);
    zth::debug::drag_float("Mouse Sensitivity", mouse_sensitivity);

    zth::debug::checkbox("Clamp Pitch", clamp_pitch);

    if (clamp_pitch)
    {
        zth::debug::slide_angle("Minimum Pitch", min_pitch, -90.0f, 0.0f);
        zth::debug::slide_angle("Maximum Pitch", max_pitch, 0.0f, 90.0f);
    }

    zth::debug::select_key("Move Forward Key", move_forward_key);
    zth::debug::select_key("Move Backward Key", move_backward_key);
    zth::debug::select_key("Move Left Key", move_left_key);
    zth::debug::select_key("Move Right Key", move_right_key);

    zth::debug::checkbox("Sprinting Enabled", sprinting_enabled);

    if (sprinting_enabled)
    {
        zth::debug::select_key("Sprint Key", sprint_key);
        zth::debug::drag_float("Sprinting Speed Multiplier", sprinting_speed_multiplier);
    }

    zth::debug::select_key("Toggle Flashlight Key", toggle_flashlight_key);
}

auto Camera::on_event(zth::EntityHandle actor, const zth::Event& event) -> void
{
    switch (event.type())
    {
        using enum zth::EventType;
    case WindowResized:
        on_window_resized_event(actor, event.window_resized_event());
        break;
    case KeyPressed:
        on_key_pressed_event(actor, event.key_pressed_event());
        break;
    }
}

auto Camera::on_update(zth::EntityHandle actor) -> void
{
    auto& transform = actor.transform();

    {
        // Move around.

        auto speed = movement_speed * zth::Time::delta_time<float>();

        if (sprinting_enabled && zth::Input::is_key_pressed(sprint_key))
            speed *= sprinting_speed_multiplier;

        auto forward = transform.forward() * speed;
        auto backward = -forward;
        auto right = transform.right() * speed;
        auto left = -right;

        if (zth::Input::is_key_pressed(move_forward_key))
            transform.translate(forward);

        if (zth::Input::is_key_pressed(move_backward_key))
            transform.translate(backward);

        if (zth::Input::is_key_pressed(move_right_key))
            transform.translate(right);

        if (zth::Input::is_key_pressed(move_left_key))
            transform.translate(left);
    }

    if (!zth::Window::cursor_enabled())
    {
        // Look around.

        auto mouse_delta =
            zth::Input::mouse_pos_delta() / static_cast<float>(zth::Window::size().y) * mouse_sensitivity;
        auto angles = transform.euler_angles();

        angles.pitch -= mouse_delta.y;
        angles.yaw -= mouse_delta.x;
        angles.roll = 0.0f;

        if (clamp_pitch)
            angles.pitch = std::clamp(angles.pitch, min_pitch, max_pitch);

        transform.set_rotation(angles);
    }

    // @todo: Remove this ray cast test code.

    ImGui::Begin("Ray Cast");

    if (auto hit = zth::Physics::ray_cast(transform.translation(), transform.direction() * 1000.0f))
    {
        auto [body_id, hit_point] = *hit;
        auto hit_entity_id = zth::Physics::get_entity(body_id);
        auto& registry = actor.registry_unchecked();
        zth::EntityHandle hit_entity{ hit_entity_id, registry };
        zth::debug::text("Hit entity: {}", hit_entity.tag().tag);
        zth::debug::text("Hit point: {}", hit_point);
    }

    ImGui::End();
}

auto Camera::on_attach(zth::EntityHandle actor) -> void
{
    actor.try_emplace<zth::CameraComponent>();
    actor.try_emplace<zth::LightComponent>(zth::SpotLight{
        .properties = {
            .color = zth::colors::black,
        },
        .attenuation = {
            .linear = 0.09f,
            .quadratic = 0.032f,
        },
    });
}

auto Camera::on_window_resized_event(zth::EntityHandle actor, const zth::WindowResizedEvent& event) -> void
{
    auto [x, y] = event.new_size;

    if (auto camera = actor.try_get<zth::CameraComponent>())
        camera->get().aspect_ratio = static_cast<float>(x) / static_cast<float>(y);
}

auto Camera::on_key_pressed_event(zth::EntityHandle actor, const zth::KeyPressedEvent& event) -> void
{
    if (event.key == toggle_flashlight_key)
    {
        _flashlight_on = !_flashlight_on;

        if (auto light = actor.try_get<zth::LightComponent>())
        {
            if (light->get().type() == zth::LightType::Spot)
                light->get().spot_light().properties.color = _flashlight_on ? zth::colors::white : zth::colors::black;
        }
    }
}

} // namespace scripts
