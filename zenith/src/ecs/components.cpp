#include "zenith/ecs/components.hpp"

#include <glm/ext/matrix_clip_space.hpp>

#include "zenith/core/assert.hpp"
#include "zenith/math/matrix.hpp"
#include "zenith/math/vector.hpp"

namespace zth {

// --------------------------- TagComponent ---------------------------

auto TagComponent::display_label() -> const char*
{
    return "Tag";
}

// --------------------------- TransformComponent ---------------------------

TransformComponent::TransformComponent(glm::vec3 translation)
{
    set_translation(translation);
}

TransformComponent::TransformComponent(glm::vec3 translation, glm::vec3 direction) : TransformComponent(translation)
{
    set_direction(direction);
}

TransformComponent::TransformComponent(glm::vec3 translation, glm::quat rotation) : TransformComponent(translation)
{
    set_rotation(rotation);
}

TransformComponent::TransformComponent(glm::vec3 translation, math::AngleAxis rotation)
    : TransformComponent(translation)
{
    set_rotation(rotation);
}

TransformComponent::TransformComponent(glm::vec3 translation, math::EulerAngles rotation)
    : TransformComponent(translation)
{
    set_rotation(rotation);
}

TransformComponent::TransformComponent(glm::vec3 translation, glm::vec3 direction, glm::vec3 scale)
    : TransformComponent(translation, direction)
{
    set_scale(scale);
}

TransformComponent::TransformComponent(glm::vec3 translation, glm::quat rotation, glm::vec3 scale)
    : TransformComponent(translation, rotation)
{
    set_scale(scale);
}

TransformComponent::TransformComponent(glm::vec3 translation, math::AngleAxis rotation, glm::vec3 scale)
    : TransformComponent(translation, rotation)
{
    set_scale(scale);
}

TransformComponent::TransformComponent(glm::vec3 translation, math::EulerAngles rotation, glm::vec3 scale)
    : TransformComponent(translation, rotation)
{
    set_scale(scale);
}

TransformComponent::TransformComponent(glm::vec3 translation, glm::vec3 direction, float scale)
    : TransformComponent(translation, direction, glm::vec3{ scale })
{}

TransformComponent::TransformComponent(glm::vec3 translation, glm::quat rotation, float scale)
    : TransformComponent(translation, rotation, glm::vec3{ scale })
{}

TransformComponent::TransformComponent(glm::vec3 translation, math::AngleAxis rotation, float scale)
    : TransformComponent(translation, rotation, glm::vec3{ scale })
{}

TransformComponent::TransformComponent(glm::vec3 translation, math::EulerAngles rotation, float scale)
    : TransformComponent(translation, rotation, glm::vec3{ scale })
{}

TransformComponent::TransformComponent(const glm::mat4& transform)
{
    set_transform(transform);
}

auto TransformComponent::translate(glm::vec3 translation) -> TransformComponent&
{
    _translation += translation;
    update_transform();
    return *this;
}

auto TransformComponent::rotate(float angle, glm::vec3 axis) -> TransformComponent&
{
    _rotation = math::rotate(_rotation, angle, axis);
    update_transform();
    return *this;
}

auto TransformComponent::rotate(math::AngleAxis rotation) -> TransformComponent&
{
    _rotation = math::rotate(_rotation, rotation.angle, rotation.axis);
    update_transform();
    return *this;
}

auto TransformComponent::rotate(glm::quat rotation) -> TransformComponent&
{
    _rotation = math::rotate(_rotation, rotation);
    update_transform();
    return *this;
}

auto TransformComponent::rotate(math::EulerAngles rotation) -> TransformComponent&
{
    _rotation = math::rotate(_rotation, rotation);
    update_transform();
    return *this;
}

auto TransformComponent::scale(float factor) -> TransformComponent&
{
    return scale(glm::vec3{ factor });
}

auto TransformComponent::scale(glm::vec3 factor) -> TransformComponent&
{
    _scale *= factor;
    update_transform();
    return *this;
}

auto TransformComponent::set_translation(glm::vec3 translation) -> TransformComponent&
{
    _translation = translation;
    update_transform();
    return *this;
}

auto TransformComponent::set_rotation(float angle, glm::vec3 axis) -> TransformComponent&
{
    return set_rotation(math::to_quaternion(angle, axis));
}

auto TransformComponent::set_rotation(math::AngleAxis rotation) -> TransformComponent&
{
    return set_rotation(math::to_quaternion(rotation));
}

auto TransformComponent::set_rotation(glm::quat rotation) -> TransformComponent&
{
    _rotation = rotation;
    update_transform();
    return *this;
}

auto TransformComponent::set_rotation(math::EulerAngles rotation) -> TransformComponent&
{
    return set_rotation(math::to_quaternion(rotation));
}

auto TransformComponent::set_direction(glm::vec3 direction) -> TransformComponent&
{
    return set_rotation(math::to_quaternion(direction));
}

auto TransformComponent::set_direction(glm::vec3 direction, glm::vec3 up) -> TransformComponent&
{
    return set_rotation(math::to_quaternion(direction, up));
}

auto TransformComponent::set_scale(float scale) -> TransformComponent&
{
    return set_scale(glm::vec3{ scale });
}

auto TransformComponent::set_scale(glm::vec3 scale) -> TransformComponent&
{
    _scale = scale;
    update_transform();
    return *this;
}

auto TransformComponent::set_transform(const glm::mat4& transform) -> TransformComponent&
{
    auto [translation, rotation, scale] = math::decompose_transform(transform);

    _translation = translation;
    _rotation = rotation;
    _scale = scale;

    _transform = transform;
    return *this;
}

auto TransformComponent::euler_angles() const -> math::EulerAngles
{
    return math::to_euler_angles(_rotation);
}

auto TransformComponent::direction() const -> glm::vec3
{
    return math::to_direction(_rotation);
}

auto TransformComponent::forward() const -> glm::vec3
{
    return _rotation * math::world_forward;
}

auto TransformComponent::right() const -> glm::vec3
{
    return _rotation * math::world_right;
}

auto TransformComponent::up() const -> glm::vec3
{
    return _rotation * math::world_up;
}

auto TransformComponent::display_label() -> const char*
{
    return "Transform";
}

auto TransformComponent::update_transform() -> void
{
    _transform = math::compose_transform(_scale, _rotation, _translation);
}

// --------------------------- ScriptComponent ---------------------------

ScriptComponent::ScriptComponent(UniquePtr<Script>&& script) : _script{ std::move(script) } {}

auto ScriptComponent::script() -> Script&
{
    return *_script;
}

auto ScriptComponent::script() const -> const Script&
{
    return *_script;
}

auto ScriptComponent::display_label() -> const char*
{
    return "Script";
}

// --------------------------- Sprite2DComponent ---------------------------

auto Sprite2DComponent::display_label() -> const char*
{
    return "Sprite 2D";
}

// --------------------------- MeshComponent ---------------------------

auto MeshComponent::display_label() -> const char*
{
    return "Mesh";
}

// --------------------------- MaterialComponent ---------------------------

auto MaterialComponent::display_label() -> const char*
{
    return "Material";
}

// --------------------------- CameraComponent ---------------------------

auto CameraComponent::view(const TransformComponent& transform) const -> glm::mat4
{
    auto translation = transform.translation();
    return glm::lookAt(translation, translation + transform.forward(), transform.up());
}

auto CameraComponent::projection() const -> glm::mat4
{
    return glm::perspective(fov, aspect_ratio, near, far);
}

auto CameraComponent::view_projection(const TransformComponent& transform) const -> glm::mat4
{
    return projection() * view(transform);
}

auto CameraComponent::display_label() -> const char*
{
    return "Camera";
}

// --------------------------- LightComponent ---------------------------

LightComponent::LightComponent() : _type{ LightType::Point }, _point_light{ PointLight{} } {}

LightComponent::LightComponent(const DirectionalLight& light)
    : _type{ LightType::Directional }, _directional_light{ light }
{}

LightComponent::LightComponent(const PointLight& light) : _type{ LightType::Point }, _point_light{ light } {}

LightComponent::LightComponent(const SpotLight& light) : _type{ LightType::Spot }, _spot_light{ light } {}

LightComponent::LightComponent(const AmbientLight& light) : _type{ LightType::Ambient }, _ambient_light{ light } {}

auto LightComponent::set_light(LightType type) -> void
{
    switch (type)
    {
        using enum LightType;
    case Directional:
        set_light(DirectionalLight{});
        break;
    case Point:
        set_light(PointLight{});
        break;
    case Spot:
        set_light(SpotLight{});
        break;
    case Ambient:
        set_light(AmbientLight{});
        break;
    }
}

auto LightComponent::set_light(const DirectionalLight& light) -> void
{
    _type = LightType::Directional;
    _directional_light = light;
}

auto LightComponent::set_light(const PointLight& light) -> void
{
    _type = LightType::Point;
    _point_light = light;
}

auto LightComponent::set_light(const SpotLight& light) -> void
{
    _type = LightType::Spot;
    _spot_light = light;
}

auto LightComponent::set_light(const AmbientLight& light) -> void
{
    _type = LightType::Ambient;
    _ambient_light = light;
}

auto LightComponent::directional_light() -> DirectionalLight&
{
    ZTH_ASSERT(_type == LightType::Directional);
    return _directional_light;
}

auto LightComponent::directional_light() const -> const DirectionalLight&
{
    ZTH_ASSERT(_type == LightType::Directional);
    return _directional_light;
}

auto LightComponent::point_light() -> PointLight&
{
    ZTH_ASSERT(_type == LightType::Point);
    return _point_light;
}

auto LightComponent::point_light() const -> const PointLight&
{
    ZTH_ASSERT(_type == LightType::Point);
    return _point_light;
}

auto LightComponent::spot_light() -> SpotLight&
{
    ZTH_ASSERT(_type == LightType::Spot);
    return _spot_light;
}

auto LightComponent::spot_light() const -> const SpotLight&
{
    ZTH_ASSERT(_type == LightType::Spot);
    return _spot_light;
}

auto LightComponent::ambient_light() -> AmbientLight&
{
    ZTH_ASSERT(_type == LightType::Ambient);
    return _ambient_light;
}

auto LightComponent::ambient_light() const -> const AmbientLight&
{
    ZTH_ASSERT(_type == LightType::Ambient);
    return _ambient_light;
}

auto LightComponent::display_label() -> const char*
{
    return "Light";
}

} // namespace zth
