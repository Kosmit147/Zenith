#include "zenith/math/matrix.hpp"

#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "zenith/math/vector.hpp"

namespace zth::math {

auto compose_transform(glm::vec3 scale, glm::quat rotation, glm::vec3 translation) -> glm::mat4
{
    glm::mat4 transform = glm::translate(glm::mat4{ 1.0f }, translation);
    transform *= glm::mat4_cast(rotation);
    transform = glm::scale(transform, scale);

    return transform;
}

auto compose_transform(const TransformComponents& components) -> glm::mat4
{
    auto& [scale, rotation, translation] = components;
    return compose_transform(scale, rotation, translation);
}

auto get_translation(const glm::mat4& transform) -> glm::vec3
{
    return transform[3];
}

auto get_scale(const glm::mat4& transform) -> glm::vec3
{
    auto x = glm::length(glm::vec3{ transform[0] });
    auto y = glm::length(glm::vec3{ transform[1] });
    auto z = glm::length(glm::vec3{ transform[2] });

    return glm::vec3{ x, y, z };
}

auto has_uniform_scale(const glm::mat4& transform, float epsilon) -> bool
{
    auto scale = get_scale(transform);
    return has_relatively_equal_components(scale, epsilon);
}

auto get_normal_matrix(const glm::mat4& transform) -> glm::mat3
{
    // we can tolerate fairly big errors
    // @speed: test what epsilon value would be the best
    static constexpr auto epsilon = 1e-5f;

    if (has_uniform_scale(transform, epsilon))
        return glm::mat3{ transform };

    // the transpose of the inverse of the upper-left 3x3 part of the transform matrix
    // @speed: investigate whether caching these matrices is worth it
    return glm::inverseTranspose(glm::mat3{ transform });
}

auto decompose(const glm::mat4& matrix) -> TransformComponents
{
    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 translation;

    glm::vec3 unused_skew;
    glm::vec4 unused_perspective;
    glm::decompose(matrix, scale, rotation, translation, unused_skew, unused_perspective);

    // the resulting quaternion is incorrect, and we need to conjugate it
    // @test: check if this works correctly
    rotation = glm::conjugate(rotation);

    return TransformComponents{
        .translation = translation,
        .rotation = rotation,
        .scale = scale,
    };
}

} // namespace zth::math
