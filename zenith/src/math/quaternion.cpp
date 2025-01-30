#include "zenith/math/quaternion.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/structured_bindings.hpp>
#include <glm/trigonometric.hpp>

#include "zenith/math/vector.hpp"

namespace zth::math {

namespace {

constexpr auto reference_vector = world_forward;

} // namespace

auto to_quaternion(float angle, glm::vec3 axis) -> glm::quat
{
    return glm::rotate(glm::identity<glm::quat>(), angle, axis);
}

auto to_quaternion(Rotation rotation) -> glm::quat
{
    return to_quaternion(rotation.angle, rotation.axis);
}

auto to_quaternion(EulerAngles angles) -> glm::quat
{
    return glm::quat{ glm::vec3{ angles.pitch, angles.yaw, angles.roll } };
}

auto to_rotation(glm::vec3 direction) -> Rotation
{
    // @robustness: handle cases where direction vector is opposite to the reference vector
    // in that case the resulting axis vector is (0, 0, 0)
    auto axis = glm::cross(reference_vector, direction);
    auto angle = glm::acos(glm::dot(reference_vector, direction));
    return Rotation{ .angle = angle, .axis = axis };
}

auto to_direction(glm::quat rotation) -> glm::vec3
{
    return rotation * reference_vector;
}

auto to_direction(EulerAngles angles) -> glm::vec3
{
    auto [pitch, yaw, _] = angles;

    auto x = glm::cos(pitch) * glm::cos(yaw);
    auto y = glm::sin(pitch);
    auto z = glm::cos(pitch) * glm::sin(yaw);

    return glm::normalize(glm::vec3{ x, y, z });
}

auto to_euler_angles(glm::quat rotation) -> EulerAngles
{
    auto [x, y, z] = glm::eulerAngles(rotation);

    return EulerAngles{
        .pitch = x,
        .yaw = y,
        .roll = z,
    };
}

auto to_euler_angles(glm::vec3 direction) -> EulerAngles
{
    auto [x, y, z] = direction;

    return EulerAngles{
        .pitch = glm::asin(y),
        .yaw = std::atan2(z, x),
        .roll = 0.0f, // a direction vector does not define roll
    };
}

auto rotate(glm::quat quaternion, glm::quat rotation) -> glm::quat
{
    return rotation * quaternion;
}

auto rotate(glm::quat quaternion, float angle, glm::vec3 axis) -> glm::quat
{
    return glm::normalize(glm::rotate(quaternion, angle, axis));
}

auto rotate(glm::quat quaternion, Rotation rotation) -> glm::quat
{
    return rotate(quaternion, rotation.angle, rotation.axis);
}

auto rotate(glm::quat quaternion, EulerAngles rotation) -> glm::quat
{
    return rotate(quaternion, to_quaternion(rotation));
}

} // namespace zth::math
