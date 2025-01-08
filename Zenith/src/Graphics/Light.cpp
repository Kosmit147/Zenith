#include "Zenith/Graphics/Light.hpp"

namespace zth {

Light::Light(glm::vec3 color) : properties{ .color = color } {}

Light::Light(const LightProperties& properties) : properties{ properties } {}

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 color) : Light(color)
{
    set_direction(direction);
}

DirectionalLight::DirectionalLight(glm::vec3 direction, const LightProperties& properties) : Light(properties)
{
    set_direction(direction);
}

PointLight::PointLight(glm::vec3 position, glm::vec3 color) : Light(color)
{
    set_translation(position);
}

PointLight::PointLight(glm::vec3 position, const LightProperties& properties) : Light(properties)
{
    set_translation(position);
}

} // namespace zth
