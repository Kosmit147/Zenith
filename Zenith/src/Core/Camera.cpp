#include "Zenith/Core/Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace zth {

Camera::Camera(glm::vec3 position, glm::vec3 front, float aspect_ratio, float fov)
    : position(position), aspect_ratio(aspect_ratio), fov(fov), _front(front)
{
    update_right();
    update_up();

    _pitch = glm::asin(front.y);
    _yaw = std::atan2(front.z, front.y);
}

Camera::Camera(glm::vec3 position, float yaw, float pitch, float aspect_ratio, float fov)
    : position(position), aspect_ratio(aspect_ratio), fov(fov), _yaw(yaw), _pitch(pitch)
{
    update_direction_vectors();
}

auto Camera::view() const -> glm::mat4
{
    return lookAt(position, position + _front, _up);
}

auto Camera::projection() const -> glm::mat4
{
    return glm::perspective(fov, aspect_ratio, 0.1f, 100.0f);
}

auto Camera::set_yaw(float yaw) -> void
{
    _yaw = yaw;
    update_direction_vectors();
}

auto Camera::set_pitch(float pitch) -> void
{
    _pitch = pitch;
    update_direction_vectors();
}

auto Camera::set_yaw_and_pitch(float yaw, float pitch) -> void
{
    _yaw = yaw;
    _pitch = pitch;
    update_direction_vectors();
}

auto Camera::update_front() -> void
{
    _front.x = glm::cos(_yaw) * glm::cos(_pitch);
    _front.y = glm::sin(_pitch);
    _front.z = glm::sin(_yaw) * glm::cos(_pitch);
    _front = normalize(_front);
}

auto Camera::update_right() -> void
{
    _right = normalize(cross(_front, glm::vec3(0.0f, 1.0f, 0.0f)));
}

auto Camera::update_up() -> void
{
    _up = normalize(cross(_right, _front));
}

auto Camera::update_direction_vectors() -> void
{
    update_front();
    update_right();
    update_up();
}

} // namespace zth
