#pragma once

#include <glm/vec3.hpp>

namespace zth {

struct LightProperties
{
    glm::vec3 color = glm::vec3{ 1.0f };
    glm::vec3 ambient = glm::vec3{ 0.2f };
    glm::vec3 diffuse = glm::vec3{ 0.5f };
    glm::vec3 specular = glm::vec3{ 1.0f };
};

struct DirectionalLight
{
    glm::vec3 direction{ 0.0f, -1.0f, 0.0f };
    LightProperties properties{};
};

struct PointLight
{
    glm::vec3 position{ 0.0f };
    LightProperties properties{};
};

struct SpotLight
{
    // TODO
};

struct AmbientLight
{
    // TODO
};

} // namespace zth
