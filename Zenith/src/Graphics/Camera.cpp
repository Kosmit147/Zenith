#include "Zenith/Graphics/Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace zth {

PerspectiveCamera::PerspectiveCamera(glm::vec3 position, glm::vec3 front, float aspect_ratio, float fov)
    : _position(position), _aspect_ratio(aspect_ratio), _fov(fov), _front(front)
{
    update_right();
    update_up();

    _pitch = glm::asin(front.y);
    _yaw = std::atan2(front.z, front.y);

    update_view_projection();
}

PerspectiveCamera::PerspectiveCamera(glm::vec3 position, float yaw, float pitch, float aspect_ratio, float fov)
    : _position(position), _aspect_ratio(aspect_ratio), _fov(fov), _yaw(yaw), _pitch(pitch)
{
    update_direction_vectors();
    update_view_projection();
}

auto PerspectiveCamera::update_view_projection() -> void
{
    auto view = lookAt(_position, _position + _front, _up);
    auto projection = glm::perspective(_fov, _aspect_ratio, 0.1f, 100.0f);
    _view_projection = projection * view;
}

auto PerspectiveCamera::set_position(glm::vec3 position) -> void
{
    _position = position;
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

auto PerspectiveCamera::set_yaw(float yaw) -> void
{
    _yaw = yaw;
    update_direction_vectors();
    update_view_projection();
}

auto PerspectiveCamera::set_pitch(float pitch) -> void
{
    _pitch = pitch;
    update_direction_vectors();
    update_view_projection();
}

auto PerspectiveCamera::set_yaw_and_pitch(float yaw, float pitch) -> void
{
    _yaw = yaw;
    _pitch = pitch;
    update_direction_vectors();
    update_view_projection();
}

auto PerspectiveCamera::update_front() -> void
{
    _front.x = glm::cos(_yaw) * glm::cos(_pitch);
    _front.y = glm::sin(_pitch);
    _front.z = glm::sin(_yaw) * glm::cos(_pitch);
    _front = normalize(_front);
}

auto PerspectiveCamera::update_right() -> void
{
    _right = normalize(cross(_front, glm::vec3(0.0f, 1.0f, 0.0f)));
}

auto PerspectiveCamera::update_up() -> void
{
    _up = normalize(cross(_right, _front));
}

auto PerspectiveCamera::update_direction_vectors() -> void
{
    update_front();
    update_right();
    update_up();
}

} // namespace zth
