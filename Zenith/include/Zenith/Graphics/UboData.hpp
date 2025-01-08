#pragma once

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <cstddef>

#include "Zenith/Utility/Macros.hpp"

namespace zth {

#define ZTH_UBO_PAD(bytes)                                                                                             \
    std::byte ZTH_UNIQUE_NAME(pad)[bytes] {}

struct CameraUboData
{
    glm::mat4 view_projection;
    glm::vec3 camera_position;
};

struct LightUboData
{
    struct LightProperties
    {
        glm::vec3 color;
        ZTH_UBO_PAD(4);
        glm::vec3 ambient;
        ZTH_UBO_PAD(4);
        glm::vec3 diffuse;
        ZTH_UBO_PAD(4);
        glm::vec3 specular;
        ZTH_UBO_PAD(4);
    };

    glm::vec3 directional_light_direction;
    ZTH_UBO_PAD(4);
    LightProperties directional_light_properties;

    glm::vec3 point_light_position;
    ZTH_UBO_PAD(4);
    LightProperties point_light_properties;

    GLboolean has_directional_light = false;
    ZTH_UBO_PAD(3);
    GLboolean has_point_light = false;
    ZTH_UBO_PAD(3);
};

struct MaterialUboData
{
    struct Material
    {
        glm::vec3 albedo;
        ZTH_UBO_PAD(4);
        glm::vec3 ambient;
        ZTH_UBO_PAD(4);
        glm::vec3 diffuse;
        ZTH_UBO_PAD(4);
        glm::vec3 specular;
        GLfloat shininess;
    };

    Material material;

    GLboolean has_diffuse_map = false;
    ZTH_UBO_PAD(3);
    GLboolean has_specular_map = false;
    ZTH_UBO_PAD(3);
    GLboolean has_emission_map = false;
    ZTH_UBO_PAD(3);
};

} // namespace zth
