#include "zenith/ecs/components.hpp"

#include "zenith/core/assert.hpp"
#include "zenith/log/formatters.hpp"
#include "zenith/math/matrix.hpp"

namespace zth {

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

TransformComponent::TransformComponent(glm::vec3 translation, math::Rotation rotation) : TransformComponent(translation)
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

TransformComponent::TransformComponent(glm::vec3 translation, math::Rotation rotation, glm::vec3 scale)
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

TransformComponent::TransformComponent(glm::vec3 translation, math::Rotation rotation, float scale)
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

auto TransformComponent::rotate(math::Rotation rotation) -> TransformComponent&
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

auto TransformComponent::set_rotation(math::Rotation rotation) -> TransformComponent&
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

auto TransformComponent::direction() const -> glm::vec3
{
    return math::to_direction(_rotation);
}

auto TransformComponent::update_transform() -> void
{
    _transform = math::compose_transform(_scale, _rotation, _translation);
}

// --------------------------- LightComponent ---------------------------

LightComponent::LightComponent() : _type(LightType::Point), _point_light(PointLight{}) {}

LightComponent::LightComponent(const DirectionalLight& light) : _type(LightType::Directional), _directional_light(light)
{}

LightComponent::LightComponent(const PointLight& light) : _type(LightType::Point), _point_light(light) {}

LightComponent::LightComponent(const SpotLight& light) : _type(LightType::Spot), _spot_light(light) {}

LightComponent::LightComponent(const AmbientLight& light) : _type(LightType::Ambient), _ambient_light(light) {}

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

auto LightComponent::to_directional_light_render_data(const TransformComponent& transform) const
    -> DirectionalLightRenderData
{
    const auto& [properties] = directional_light();

    return DirectionalLightRenderData{
        .direction = transform.direction(),
        .properties = properties,
    };
}

auto LightComponent::to_point_light_render_data(const TransformComponent& transform) const -> PointLightRenderData
{
    const auto& [properties, attenuation] = point_light();

    return PointLightRenderData{
        .position = transform.translation(),
        .properties = properties,
        .attenuation = attenuation,
    };
}

auto LightComponent::to_spot_light_render_data(const TransformComponent& transform) const -> SpotLightRenderData
{
    const auto& [inner_cutoff_cosine, outer_cutoff_cosine, properties, attenuation] = spot_light();

    return SpotLightRenderData{
        .position = transform.translation(),
        .direction = transform.direction(),
        .inner_cutoff_cosine = inner_cutoff_cosine,
        .outer_cutoff_cosine = outer_cutoff_cosine,
        .properties = properties,
        .attenuation = attenuation,
    };
}

auto LightComponent::to_ambient_light_render_data() const -> AmbientLightRenderData
{
    const auto& [ambient] = ambient_light();

    return AmbientLightRenderData{
        .ambient = ambient,
    };
}

} // namespace zth

ZTH_DEFINE_FORMATTER(zth::TagComponent, tag)
{
    return ZTH_FORMAT_OUT("TagComponent {{\n"
                          "\t.tag = {},\n"
                          "}}",
                          tag.tag);
}

ZTH_DEFINE_FORMATTER(zth::TransformComponent, transform)
{
    return ZTH_FORMAT_OUT("TransformComponent {{\n"
                          "\t.translation = {},\n"
                          "\t.rotation = {},\n"
                          "\t.scale = {},\n"
                          "\t.transform = {},\n"
                          "}}",
                          transform.translation(), transform.rotation(), transform.scale(), transform.transform());
}

ZTH_DEFINE_FORMATTER(zth::MeshComponent, mesh)
{
    // @todo

    (void)mesh;
    return ZTH_FORMAT_OUT("not implemented");
}

ZTH_DEFINE_FORMATTER(zth::MaterialComponent, material)
{
    // @todo

    (void)material;
    return ZTH_FORMAT_OUT("not implemented");
}

ZTH_DEFINE_FORMATTER(zth::CameraComponent, camera)
{
    // @todo

    (void)camera;
    return ZTH_FORMAT_OUT("not implemented");
}

ZTH_DEFINE_FORMATTER(zth::LightComponent, light)
{
    ZTH_FORMAT_OUT("LightComponent {{\n"
                   "\t.type = {},\n",
                   light.type());

    switch (light.type())
    {
        using enum zth::LightType;
    case Directional:
        ZTH_FORMAT_OUT("\t.directional_light = {},\n", light.directional_light());
        break;
    case Point:
        ZTH_FORMAT_OUT("\t.point_light = {},\n", light.point_light());
        break;
    case Spot:
        ZTH_FORMAT_OUT("\t.spot_light = {},\n", light.spot_light());
        break;
    case Ambient:
        ZTH_FORMAT_OUT("\t.ambient_light = {},\n", light.ambient_light());
        break;
    }

    return ZTH_FORMAT_OUT("}}");
}
