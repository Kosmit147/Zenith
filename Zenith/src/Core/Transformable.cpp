#include "Zenith/Core/Transformable.hpp"

#include "Zenith/Math/Matrix.hpp"
#include "Zenith/Math/Quaternion.hpp"

namespace zth {

Transformable3D::Transformable3D(glm::vec3 translation)
{
    set_translation(translation);
}

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
    _rotation = math::rotate(_rotation, angle, axis);
    update_transform();
    return *this;
}

auto Transformable3D::rotate(math::Rotation rotation) -> Transformable3D&
{
    return rotate(rotation.angle, rotation.axis);
}

auto Transformable3D::rotate(glm::quat rotation) -> Transformable3D&
{
    _rotation = math::rotate(_rotation, rotation);
    update_transform();
    return *this;
}

auto Transformable3D::rotate(math::EulerAngles rotation) -> Transformable3D&
{
    _rotation = math::rotate(_rotation, rotation);
    update_transform();
    return *this;
}

auto Transformable3D::scale(float factor) -> Transformable3D&
{
    _scale *= factor;
    update_transform();
    return *this;
}

auto Transformable3D::scale(glm::vec3 factor) -> Transformable3D&
{
    _scale *= factor;
    update_transform();
    return *this;
}

auto Transformable3D::set_translation(glm::vec3 translation) -> Transformable3D&
{
    _translation = translation;
    update_transform();
    return *this;
}

auto Transformable3D::set_rotation(float angle, glm::vec3 axis) -> Transformable3D&
{
    _rotation = math::to_quaternion(angle, axis);
    update_transform();
    return *this;
}

auto Transformable3D::set_rotation(math::Rotation rotation) -> Transformable3D&
{
    return set_rotation(rotation.angle, rotation.axis);
}

auto Transformable3D::set_rotation(glm::quat rotation) -> Transformable3D&
{
    _rotation = rotation;
    update_transform();
    return *this;
}

auto Transformable3D::set_rotation(math::EulerAngles rotation) -> Transformable3D&
{
    _rotation = math::to_quaternion(rotation);
    update_transform();
    return *this;
}

auto Transformable3D::set_direction(glm::vec3 direction) -> Transformable3D&
{
    return set_rotation(math::to_rotation(direction));
}

auto Transformable3D::set_scale(float scale) -> Transformable3D&
{
    _scale = glm::vec3{ scale };
    update_transform();
    return *this;
}

auto Transformable3D::set_scale(glm::vec3 scale) -> Transformable3D&
{
    _scale = scale;
    update_transform();
    return *this;
}

auto Transformable3D::set_transform(const glm::mat4& transform) -> Transformable3D&
{
    auto [translation, rotation, scale] = math::decompose(transform);

    _translation = translation;
    _rotation = rotation;
    _scale = scale;

    _transform = transform;
    return *this;
}

auto Transformable3D::direction() const -> glm::vec3
{
    return math::to_direction(_rotation);
}

auto Transformable3D::update_transform() -> void
{
    _transform = math::compose_transform(_scale, _rotation, _translation);
}

} // namespace zth
