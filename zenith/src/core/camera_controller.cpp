#include "zenith/core/camera_controller.hpp"

#include "zenith/core/assert.hpp"
#include "zenith/platform/input.hpp"
#include "zenith/time/time.hpp"

#include "zenith/math/quaternion.hpp"

namespace zth {

FpsCameraController::FpsCameraController(std::shared_ptr<PerspectiveCamera> camera) : _camera(std::move(camera)) {}

auto FpsCameraController::on_update() const -> void
{
    ZTH_ASSERT(_camera != nullptr);

    auto delta_time = Time::delta_time<float>();
    auto speed = movement_speed;

    if (sprint_enabled)
    {
        if (Input::is_key_pressed(sprint_key))
            speed *= sprint_speed_multiplier;
    }

    auto new_camera_pos = _camera->translation();

    if (Input::is_key_pressed(move_forward_key))
        new_camera_pos += _camera->forward() * speed * delta_time;

    if (Input::is_key_pressed(move_back_key))
        new_camera_pos -= _camera->forward() * speed * delta_time;

    if (Input::is_key_pressed(move_left_key))
        new_camera_pos -= _camera->right() * speed * delta_time;

    if (Input::is_key_pressed(move_right_key))
        new_camera_pos += _camera->right() * speed * delta_time;

    auto mouse_delta = Input::mouse_pos_delta();
    mouse_delta *= sensitivity;

    auto new_yaw = _camera->yaw() + mouse_delta.x;
    auto new_pitch = std::clamp(_camera->pitch() - mouse_delta.y, min_pitch, max_pitch);

    _camera->set_translation_and_rotation(new_camera_pos, math::EulerAngles{ .pitch = new_pitch, .yaw = new_yaw });
}

auto FpsCameraController::set_camera(std::shared_ptr<PerspectiveCamera> camera) -> void
{
    ZTH_ASSERT(camera != nullptr);
    _camera = std::move(camera);
}

} // namespace zth
