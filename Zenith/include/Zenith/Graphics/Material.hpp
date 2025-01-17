#pragma once

#include <glm/vec3.hpp>

#include "Zenith/Graphics/Shaders.hpp"
#include "Zenith/Platform/OpenGl/fwd.hpp"

namespace zth {

struct Material
{
    const Shader* shader = &shaders::standard();

    glm::vec3 albedo = glm::vec3{ 1.0f };
    const Texture2D* diffuse_map = nullptr;
    const Texture2D* specular_map = nullptr;
    const Texture2D* emission_map = nullptr;

    glm::vec3 ambient = glm::vec3{ 0.2f };
    glm::vec3 diffuse = glm::vec3{ 1.0f };
    glm::vec3 specular = glm::vec3{ 0.6f };
    float shininess = 32.0f;
};

} // namespace zth
