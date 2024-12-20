#include "Zenith/Core/CameraController.hpp"

#include "Zenith/Core/Assert.hpp"
#include "Zenith/Platform/Input.hpp"
#include "Zenith/Time/Time.hpp"

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

    auto new_camera_pos = _camera->position();

    if (Input::is_key_pressed(move_forward_key))
        new_camera_pos += _camera->front() * speed * delta_time;

    if (Input::is_key_pressed(move_back_key))
        new_camera_pos -= _camera->front() * speed * delta_time;

    if (Input::is_key_pressed(move_left_key))
        new_camera_pos -= _camera->right() * speed * delta_time;

    if (Input::is_key_pressed(move_right_key))
        new_camera_pos += _camera->right() * speed * delta_time;

    _camera->set_position(new_camera_pos);

    auto mouse_delta = Input::mouse_pos_delta();
    mouse_delta *= sensitivity;

    auto new_yaw = _camera->yaw() + mouse_delta.x;
    auto new_pitch = std::clamp(_camera->pitch() - mouse_delta.y, min_pitch, max_pitch);

    _camera->set_yaw_and_pitch(new_yaw, new_pitch);
}

auto FpsCameraController::set_camera(std::shared_ptr<PerspectiveCamera> camera) -> void
{
    ZTH_ASSERT(camera != nullptr);
    _camera = std::move(camera);
}

} // namespace zth
