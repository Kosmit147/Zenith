#include "Zenith/Core/Transformable.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace zth {

Transformable3D::Transformable3D(const glm::mat4& transform)
{
    set_transform(transform);
}

auto Transformable3D::translate(glm::vec3 translation) -> Transformable3D&
{
    _translation += translation;
    update_transform();
    return *this;
}

auto Transformable3D::rotate(float angle, glm::vec3 axis) -> Transformable3D&
{
    _rotation = glm::normalize(glm::rotate(_rotation, angle, axis));
    update_transform();
    return *this;
}

auto Transformable3D::scale(glm::vec3 factor) -> Transformable3D&
{
    _scale *= factor;
    update_transform();
    return *this;
}

auto Transformable3D::set_transform(const glm::mat4& transform) -> void
{
    glm::vec3 unused_skew;
    glm::vec4 unused_perspective;
    glm::decompose(transform, _scale, _rotation, _translation, unused_skew, unused_perspective);

    // the resulting quaternion is incorrect, and we need to conjugate it
    // TODO: check if this works correctly
    _rotation = glm::conjugate(_rotation);

    _transform = transform;
}

auto Transformable3D::update_transform() -> void
{
    _transform = glm::translate(glm::mat4{ 1.0f }, _translation);
    _transform *= glm::mat4_cast(_rotation);
    _transform = glm::scale(_transform, _scale);
}

} // namespace zth
