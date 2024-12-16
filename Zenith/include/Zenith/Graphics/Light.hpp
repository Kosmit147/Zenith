#pragma once

#include <glm/vec3.hpp>

#include "Zenith/Core/Transformable.hpp"
#include "Zenith/Utility/Utility.hpp"

namespace zth {

class Light : public Transformable3D
{
public:
    glm::vec3 color = glm::vec3{ 1.0f };

    glm::vec3 ambient = glm::vec3{ 0.2f };
    glm::vec3 diffuse = glm::vec3{ 0.5f };
    glm::vec3 specular = glm::vec3{ 1.0f };

public:
    Light() = delete;
    ZTH_DEFAULT_COPY_DEFAULT_MOVE(Light)
    ~Light() override = default;

protected:
    explicit Light(glm::vec3 position, glm::vec3 color);
};

class PointLight : public Light
{
public:
    explicit PointLight(glm::vec3 position, glm::vec3 color);
};

class DirectionalLight : public Light
{
    // TODO
};

class SpotLight : public Light
{
    // TODO
};

} // namespace zth
