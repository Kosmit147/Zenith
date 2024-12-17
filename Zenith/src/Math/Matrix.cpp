#include "Zenith/Math/Matrix.hpp"

#include <glm/gtc/matrix_inverse.hpp>

namespace zth {

auto get_translation(const glm::mat4& transform) -> glm::vec3
{
    return transform[3];
}

auto get_scale(const glm::mat4& transform) -> glm::vec3
{
    // TODO: is this correct?
    // should we take the length of the columns or the rows?
    auto x = glm::length(glm::vec3{ transform[0] });
    auto y = glm::length(glm::vec3{ transform[1] });
    auto z = glm::length(glm::vec3{ transform[2] });
    return glm::vec3{ x, y, z };
}

auto has_uniform_scale(const glm::mat4& transform) -> bool
{
    // TODO: correct float comparisons

    auto scale = get_scale(transform);
    return scale.x == scale.y && scale.y == scale.z;
}

auto get_normal_matrix(const glm::mat4& transform) -> glm::mat3
{
    // TODO: correct float comparisons

    if (has_uniform_scale(transform))
        return glm::mat3{ transform };

    // the transpose of the inverse of the upper-left 3x3 part of the transform matrix
    return glm::inverseTranspose(glm::mat3{ transform });
}

} // namespace zth
