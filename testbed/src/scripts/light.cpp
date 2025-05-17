#include "light.hpp"

namespace scripts {

Light::Light(std::shared_ptr<zth::Material> light_material) : _light_material{ std::move(light_material) } {}

auto Light::on_update(zth::EntityHandle actor) -> void
{
    if (auto light = actor.try_get<const zth::LightComponent>())
    {
        switch (light->get().type())
        {
            using enum zth::LightType;
        case Directional:
            _light_material->albedo = light->get().directional_light().properties.color;
            break;
        case Point:
            _light_material->albedo = light->get().point_light().properties.color;
            break;
        case Spot:
            _light_material->albedo = light->get().spot_light().properties.color;
            break;
        }
    }
}

} // namespace scripts
