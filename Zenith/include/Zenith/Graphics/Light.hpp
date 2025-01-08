#pragma once

#include <glm/vec3.hpp>

#include "Zenith/Core/Transformable.hpp"
#include "Zenith/Utility/Macros.hpp"

namespace zth {

struct LightProperties
{
    glm::vec3 color = glm::vec3{ 1.0f };
    glm::vec3 ambient = glm::vec3{ 0.2f };
    glm::vec3 diffuse = glm::vec3{ 0.5f };
    glm::vec3 specular = glm::vec3{ 1.0f };
};

class Light : public Transformable3D
{
public:
    LightProperties properties{};

public:
    ZTH_DEFAULT_COPY_DEFAULT_MOVE(Light)
    ~Light() override = default;

protected:
    explicit Light() = default;
    explicit Light(glm::vec3 color);
    explicit Light(const LightProperties& properties);
};

class DirectionalLight : public Light
{
public:
    explicit DirectionalLight(glm::vec3 direction, glm::vec3 color = glm::vec3{ 1.0f });
    explicit DirectionalLight(glm::vec3 direction, const LightProperties& properties);
};

class PointLight : public Light
{
public:
    explicit PointLight(glm::vec3 position, glm::vec3 color = glm::vec3{ 1.0f });
    explicit PointLight(glm::vec3 position, const LightProperties& properties);
};

class SpotLight : public Light
{
    // TODO
};

class AmbientLight : public Light
{
    // TODO
};

} // namespace zth
