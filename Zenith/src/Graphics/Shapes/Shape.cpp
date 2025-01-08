#include "Zenith/Graphics/Shapes/Shape.hpp"

namespace zth {

Shape3D::Shape3D(glm::vec3 translation) : Transformable3D(translation) {}

Shape3D::Shape3D(const glm::mat4& transform) : Transformable3D(transform) {}

} // namespace zth
