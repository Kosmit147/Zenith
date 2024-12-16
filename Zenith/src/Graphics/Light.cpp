#include "Zenith/Graphics/Light.hpp"

namespace zth {

Light::Light(glm::vec3 position, glm::vec3 color) : Transformable3D(position), color(color) {}

PointLight::PointLight(glm::vec3 position, glm::vec3 color) : Light(position, color) {}

} // namespace zth
