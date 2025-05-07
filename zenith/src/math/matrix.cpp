#include "zenith/math/matrix.hpp"

#include <glm/geometric.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "zenith/math/vector.hpp"

namespace zth::math {

auto compose_transform(glm::vec3 scale, glm::quat rotation, glm::vec3 translation) -> glm::mat4
{
    glm::mat4 transform = glm::translate(glm::mat4{ 1.0f }, translation);
    transform *= rotation_matrix_from_quaternion(rotation);
    transform = glm::scale(transform, scale);

    return transform;
}

auto compose_transform(glm::vec3 scale, EulerAngles rotation, glm::vec3 translation) -> glm::mat4
{
    glm::mat4 transform = glm::translate(glm::mat4{ 1.0f }, translation);
    transform *= rotation_matrix_from_euler_angles(rotation);
    transform = glm::scale(transform, scale);

    return transform;
}

auto compose_transform(const TransformComponents& components) -> glm::mat4
{
    auto& [scale, rotation, translation] = components;
    return compose_transform(scale, rotation, translation);
}

auto decompose_transform(const glm::mat4& transform) -> TransformComponents
{
    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 translation;

    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(transform, scale, rotation, translation, skew, perspective);

    return TransformComponents{
        .translation = translation,
        .rotation = rotation,
        .scale = scale,
    };
}

auto extract_translation(const glm::mat4& transform) -> glm::vec3
{
    return glm::column(transform, 3);
}

auto extract_scale(const glm::mat4& transform) -> glm::vec3
{
    auto x = glm::length(glm::vec3{ transform[0] });
    auto y = glm::length(glm::vec3{ transform[1] });
    auto z = glm::length(glm::vec3{ transform[2] });

    return glm::vec3{ x, y, z };
}

auto has_uniform_scale(const glm::mat4& transform, float epsilon) -> bool
{
    auto scale = extract_scale(transform);
    return has_relatively_equal_components(scale, epsilon);
}

auto get_normal_matrix(const glm::mat4& transform) -> glm::mat3
{
    // We can tolerate fairly big errors.
    // @speed: Test what epsilon value would be best.
    static constexpr auto epsilon = 1e-5f;

    if (has_uniform_scale(transform, epsilon))
        return glm::mat3{ transform };

    // The transpose of the inverse of the upper-left 3x3 part of the transform matrix.
    // @speed: Investigate whether caching these matrices is worth it.
    return glm::inverseTranspose(glm::mat3{ transform });
}

auto rotation_matrix_from_quaternion(glm::quat rotation) -> glm::mat4
{
    return glm::mat4_cast(rotation);
}

auto rotation_matrix_from_euler_angles(EulerAngles angles) -> glm::mat4
{
    return glm::eulerAngleYXZ(angles.yaw, angles.pitch, angles.roll);
}

auto extract_euler_angles_from_rotation_matrix(const glm::mat4& rotation_matrix) -> EulerAngles
{
    EulerAngles angles;
    glm::extractEulerAngleYXZ(rotation_matrix, angles.yaw, angles.pitch, angles.roll);
    return angles;
}

} // namespace zth::math
