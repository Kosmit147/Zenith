#include "Zenith/Math/Matrix.hpp"

#include <glm/gtc/matrix_inverse.hpp>

#include "Zenith/Math/Vector.hpp"

namespace zth::math {

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
    static constexpr auto epsilon = 1e-5f;

    if (has_uniform_scale(transform, epsilon))
        return glm::mat3{ transform };

    // the transpose of the inverse of the upper-left 3x3 part of the transform matrix
    // TODO: investigate whether caching these matrices is worth it
    return glm::inverseTranspose(glm::mat3{ transform });
}

} // namespace zth::math
