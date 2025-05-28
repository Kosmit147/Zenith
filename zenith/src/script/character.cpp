#include "zenith/script/character.hpp"

#include "zenith/debug/ui.hpp"
#include "zenith/ecs/components.hpp"
#include "zenith/ecs/ecs.hpp"
#include "zenith/log/formatters.hpp"
#include "zenith/physics/physics.hpp"
#include "zenith/system/event.hpp"
#include "zenith/system/time.hpp"
#include "zenith/system/window.hpp"

namespace zth::scripts {

auto CharacterController::display_label() const -> const char*
{
    return "Character Controller";
}

auto CharacterController::debug_edit() -> void
{
    debug::drag_float("Movement Speed", movement_speed);
    debug::drag_float("Jump Speed", jump_speed);
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
    debug::select_key("Jump Key", jump_key);

    debug::checkbox("Sprinting Enabled", sprinting_enabled);

    if (sprinting_enabled)
    {
        debug::select_key("Sprint Key", sprint_key);
        debug::drag_float("Sprinting Speed Multiplier", sprinting_speed_multiplier);
    }
}

void CharacterController::on_event(EntityHandle actor, const Event& event)
{
    if (event.type() == EventType::WindowResized)
    {
        auto [new_size] = event.window_resized_event();

        if (auto camera = actor.try_get<CameraComponent>())
            camera->get().aspect_ratio = static_cast<float>(new_size.x) / static_cast<float>(new_size.y);
    }
}

auto CharacterController::on_fixed_update(EntityHandle actor) -> void
{
    debug::begin_window("Character Controller");

    auto& transform = actor.transform();

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

    if (!actor.any_of<CharacterControllerComponent>())
        return;

    auto movement_direction = JPH::Vec3::sZero();

    {
        // Move around.

        auto raw_forward = transform.forward();

        auto forward = glm::normalize(glm::vec3{ raw_forward.x, 0.0f, raw_forward.z });
        auto backward = -forward;
        auto right = glm::normalize(glm::cross(forward, math::world_up));
        auto left = -right;

        if (Input::is_key_pressed(move_forward_key))
            movement_direction += JPH::Vec3{ forward.x, forward.y, forward.z };

        if (Input::is_key_pressed(move_backward_key))
            movement_direction += JPH::Vec3{ backward.x, backward.y, backward.z };

        if (Input::is_key_pressed(move_right_key))
            movement_direction += JPH::Vec3{ right.x, right.y, right.z };

        if (Input::is_key_pressed(move_left_key))
            movement_direction += JPH::Vec3{ left.x, left.y, left.z };
    }

    debug::text("Movement Direction: Vec3({}, {}, {})", movement_direction.GetX(), movement_direction.GetY(),
                movement_direction.GetZ());

    auto& controller = actor.get<CharacterControllerComponent>();
    auto& character = controller.character;

    auto desired_velocity = movement_direction * movement_speed;

    if (sprinting_enabled && Input::is_key_pressed(sprint_key))
        desired_velocity *= sprinting_speed_multiplier;

    // True if the player intended to move
    // mAllowSliding = !inMovementDirection.IsNearZero();

    // @todo: The rotation is wrong!
    // Up should always be (0, 1, 0).

    // auto transform_rotation = transform.rotation();
    // JPH::Quat rotation{ transform_rotation.x, transform_rotation.y, transform_rotation.z, transform_rotation.w };
    // JPH::Quat character_up_rotation{ rotation.x, rotation.y, rotation.z, rotation.w };
    // character->SetUp(character_up_rotation.RotateAxisY());
    character->SetUp(JPH::Vec3::sAxisY());
    character->SetRotation(JPH::Quat::sIdentity());

    // @todo: Do we need to call this?
    character->UpdateGroundVelocity();

    // Determine new basic velocity
    auto current_vertical_velocity = character->GetLinearVelocity().Dot(character->GetUp()) * character->GetUp();
    auto ground_velocity = character->GetGroundVelocity();
    JPH::Vec3 new_velocity;
    auto moving_towards_ground = (current_vertical_velocity.GetY() - ground_velocity.GetY()) < 0.1f;

    if (character->GetGroundState() == JPH::CharacterVirtual::EGroundState::OnGround
        && !character->IsSlopeTooSteep(character->GetGroundNormal()))
    {
        // Assume velocity of ground when on ground
        new_velocity = ground_velocity;

        // Jump
        if (Input::is_key_pressed(jump_key) && moving_towards_ground)
            new_velocity += jump_speed * character->GetUp();
    }
    else
    {
        new_velocity = current_vertical_velocity;
    }

    // Gravity
    new_velocity += Physics::gravity() * Time::fixed_time_step<float>();

    // Player input
    new_velocity += desired_velocity;

    debug::text("New Velocity: Vec3({}, {}, {})", new_velocity.GetX(), new_velocity.GetY(), new_velocity.GetZ());

    // Update character velocity
    character->SetLinearVelocity(new_velocity);

    debug::end_window();
}

auto CharacterController::on_update(EntityHandle actor) -> void
{
    // @todo: Remove this ray cast test code.

    auto& transform = actor.transform();

    debug::begin_window("Ray Cast");

    if (auto hit = Physics::ray_cast(transform.translation(), transform.direction() * 1000.0f))
    {
        auto [body_id, hit_point] = *hit;
        auto hit_entity_id = Physics::get_entity(body_id);
        auto& registry = actor.registry_unchecked();
        EntityHandle hit_entity{ hit_entity_id, registry };
        debug::text("Hit entity: {}", hit_entity.tag().tag);
        debug::text("Hit point: {}", hit_point);
    }

    debug::end_window();
}

auto CharacterController::on_attach(EntityHandle actor) -> void
{
    actor.try_emplace<CharacterControllerComponent>();
}

} // namespace zth::scripts
