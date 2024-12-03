#include "Zenith/Core/Transformable.hpp"

#include <glm/ext/matrix_transform.hpp>

namespace zth {

auto Transformable::translate(glm::vec3 translation) -> Transformable&
{
    _translation += translation;
    update_transform();
    return *this;
}

auto Transformable::rotate(float angle, glm::vec3 axis) -> Transformable&
{
    _rotation = glm::rotate(_rotation, angle, axis);
    update_transform();
    return *this;
}

auto Transformable::scale(glm::vec3 factor) -> Transformable&
{
    _scale *= factor;
    update_transform();
    return *this;
}

auto Transformable::set_transform(const glm::mat4& transform) -> void
{
    _transform = transform;
}

auto Transformable::update_transform() -> void
{
    _transform = glm::translate(glm::mat4{ 1.0f }, _translation);
    _transform *= glm::mat4_cast(_rotation);
    _transform = glm::scale(_transform, _scale);
}

} // namespace zth
