#include "zenith/graphics/camera.hpp"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "zenith/math/quaternion.hpp"
#include "zenith/math/vector.hpp"

namespace zth {

PerspectiveCamera::PerspectiveCamera(glm::vec3 translation, glm::vec3 direction, float aspect_ratio, float fov)
    : _aspect_ratio(aspect_ratio), _fov(fov), _translation(translation), _forward(direction)
{
    auto [pitch, yaw, roll] = math::to_euler_angles(direction);

    _pitch = pitch;
    _yaw = yaw;
    _roll = roll;

    update_direction_vectors();
    update_view_projection();
}

PerspectiveCamera::PerspectiveCamera(glm::vec3 translation, math::EulerAngles rotation, float aspect_ratio, float fov)
    : _aspect_ratio(aspect_ratio), _fov(fov), _translation(translation), _pitch(rotation.pitch), _yaw(rotation.yaw),
      _roll(rotation.roll)
{
    update_direction_vectors();
    update_view_projection();
}

auto PerspectiveCamera::set_aspect_ratio(float aspect_ratio) -> void
{
    _aspect_ratio = aspect_ratio;
    update_view_projection();
}

auto PerspectiveCamera::set_fov(float fov) -> void
{
    _fov = fov;
    update_view_projection();
}

auto PerspectiveCamera::translate(glm::vec3 translation) -> PerspectiveCamera&
{
    _translation += translation;
    update_direction_vectors();
    update_view_projection();
    return *this;
}

auto PerspectiveCamera::rotate(float pitch, float yaw, float roll) -> PerspectiveCamera&
{
    _pitch += pitch;
    _yaw += yaw;
    _roll += roll;
    update_direction_vectors();
    update_view_projection();
    return *this;
}

auto PerspectiveCamera::rotate(math::EulerAngles rotation) -> PerspectiveCamera&
{
    return rotate(rotation.pitch, rotation.yaw, rotation.roll);
}

auto PerspectiveCamera::set_translation(glm::vec3 translation) -> PerspectiveCamera&
{
    _translation = translation;
    update_direction_vectors();
    update_view_projection();
    return *this;
}

auto PerspectiveCamera::set_rotation(float pitch, float yaw, float roll) -> PerspectiveCamera&
{
    _pitch = pitch;
    _yaw = yaw;
    _roll = roll;
    update_direction_vectors();
    update_view_projection();
    return *this;
}

auto PerspectiveCamera::set_rotation(math::EulerAngles rotation) -> PerspectiveCamera&
{
    return set_rotation(rotation.pitch, rotation.yaw, rotation.roll);
}

auto PerspectiveCamera::set_translation_and_rotation(glm::vec3 translation,
                                                     math::EulerAngles rotation) -> PerspectiveCamera&
{
    _translation = translation;
    _pitch = rotation.pitch;
    _yaw = rotation.yaw;
    _roll = rotation.roll;
    update_direction_vectors();
    update_view_projection();
    return *this;
}

auto PerspectiveCamera::rotation() const -> math::EulerAngles
{
    return math::EulerAngles{
        .pitch = _pitch,
        .yaw = _yaw,
        .roll = _roll,
    };
}

auto PerspectiveCamera::update_direction_vectors() -> void
{
    auto euler_angles = math::EulerAngles{
        .pitch = _pitch,
        .yaw = _yaw,
        .roll = 0.0f, // roll doesn't influence forward vector's value
    };

    _forward = math::to_direction(euler_angles);
    _right = glm::normalize(glm::cross(_forward, math::world_up));
    _up = glm::normalize(glm::cross(_right, _forward));
}

auto PerspectiveCamera::update_view_projection() -> void
{
    _view = glm::lookAt(_translation, _translation + _forward, _up);
    _projection = glm::perspective(_fov, _aspect_ratio, 0.1f, 100.0f);
    _view_projection = _projection * _view;
}

} // namespace zth
