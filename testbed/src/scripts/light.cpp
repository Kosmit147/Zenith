#include "light.hpp"

namespace scripts {

Light::Light(std::shared_ptr<zth::Material> light_material) : _light_material{ std::move(light_material) } {}

auto Light::on_update(zth::EntityHandle actor) -> void
{
    if (!actor.any_of<const zth::LightComponent>())
        return;

    auto& light = actor.get<const zth::LightComponent>();

    switch (light.type())
    {
        using enum zth::LightType;
    case Directional:
        _light_material->albedo = light.directional_light().properties.color;
        break;
    case Point:
        _light_material->albedo = light.point_light().properties.color;
        break;
    case Spot:
        _light_material->albedo = light.spot_light().properties.color;
        break;
    }
}

} // namespace scripts
