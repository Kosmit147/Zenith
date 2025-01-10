#pragma once

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "Zenith/Platform/OpenGl/GlUtils.hpp"

namespace zth {

#define ZTH_UBO_FIELD(type, name) alignas(get_std140_ubo_field_alignment<type>()) type name

struct CameraUboData
{
    ZTH_UBO_FIELD(glm::mat4, view_projection);
    ZTH_UBO_FIELD(glm::vec3, camera_position);
};

struct LightUboData
{
    struct LightProperties
    {
        ZTH_UBO_FIELD(glm::vec3, color);
        ZTH_UBO_FIELD(glm::vec3, ambient);
        ZTH_UBO_FIELD(glm::vec3, diffuse);
        ZTH_UBO_FIELD(glm::vec3, specular);
    };

    ZTH_UBO_FIELD(glm::vec3, directional_light_direction);
    LightProperties directional_light_properties;

    ZTH_UBO_FIELD(glm::vec3, point_light_position);
    LightProperties point_light_properties;

    ZTH_UBO_FIELD(GlslBool, has_directional_light = false);
    ZTH_UBO_FIELD(GlslBool, has_point_light = false);
};

struct MaterialUboData
{
    struct Material
    {
        ZTH_UBO_FIELD(glm::vec3, albedo);
        ZTH_UBO_FIELD(glm::vec3, ambient);
        ZTH_UBO_FIELD(glm::vec3, diffuse);
        ZTH_UBO_FIELD(glm::vec3, specular);
        ZTH_UBO_FIELD(GLfloat, shininess);
    };

    Material material;

    ZTH_UBO_FIELD(GlslBool, has_diffuse_map = false);
    ZTH_UBO_FIELD(GlslBool, has_specular_map = false);
    ZTH_UBO_FIELD(GlslBool, has_emission_map = false);
};

} // namespace zth
