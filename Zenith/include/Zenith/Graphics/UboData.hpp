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
    glm::vec3 light_position;
    ZTH_UBO_PAD(4);
    glm::vec3 light_color;
    ZTH_UBO_PAD(4);

    glm::vec3 light_ambient;
    ZTH_UBO_PAD(4);
    glm::vec3 light_diffuse;
    ZTH_UBO_PAD(4);
    glm::vec3 light_specular;
};

struct MaterialUboData
{
    glm::vec3 albedo;
    ZTH_UBO_PAD(4);

    glm::vec3 material_ambient;
    ZTH_UBO_PAD(4);
    glm::vec3 material_diffuse;
    ZTH_UBO_PAD(4);
    glm::vec3 material_specular;
    GLfloat material_shininess;

    GLboolean has_diffuse_map;
    ZTH_UBO_PAD(3);
    GLboolean has_specular_map;
    ZTH_UBO_PAD(3);
};

} // namespace zth
