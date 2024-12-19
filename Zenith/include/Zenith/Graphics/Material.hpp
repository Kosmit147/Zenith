#pragma once

#include <glm/vec3.hpp>

#include "Zenith/Graphics/Shaders.hpp"
#include "Zenith/Platform/OpenGl/fwd.hpp"

namespace zth {

struct Material
{
    const Shader* shader = &shaders::standard();

    glm::vec3 albedo = glm::vec3{ 1.0f };
    const Texture2D* texture = nullptr;

    glm::vec3 ambient = glm::vec3{ 0.1f };
    glm::vec3 diffuse = glm::vec3{ 0.7f };
    glm::vec3 specular = glm::vec3{ 0.4f };
    float shininess = 32.0f;
};

} // namespace zth
