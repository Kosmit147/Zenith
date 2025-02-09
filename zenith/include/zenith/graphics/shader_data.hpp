#pragma once

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "zenith/gl/util.hpp"

namespace zth {

#define ZTH_UBO_FIELD(type, name) alignas(::zth::gl::get_std140_field_alignment<type>()) type name
#define ZTH_UBO_STRUCT(name) struct alignas(16) name

#define ZTH_SSBO_FIELD(type, name) alignas(::zth::gl::get_std430_field_alignment<type>()) type name
#define ZTH_SSBO_STRUCT(name) struct name

using GlslBool = gl::GlslBool;

struct CameraUboData
{
    ZTH_UBO_FIELD(glm::mat4, view_projection);
    ZTH_UBO_FIELD(glm::vec3, camera_position);
};

struct LightSsboData
{
    ZTH_SSBO_STRUCT(LightProperties)
    {
        ZTH_SSBO_FIELD(glm::vec3, color);
        ZTH_SSBO_FIELD(glm::vec3, ambient);
        ZTH_SSBO_FIELD(glm::vec3, diffuse);
        ZTH_SSBO_FIELD(glm::vec3, specular);
    };

    ZTH_SSBO_STRUCT(LightAttenuation)
    {
        ZTH_SSBO_FIELD(GLfloat, constant);
        ZTH_SSBO_FIELD(GLfloat, linear);
        ZTH_SSBO_FIELD(GLfloat, quadratic);
    };

    ZTH_SSBO_FIELD(glm::vec3, directional_light_direction);
    LightProperties directional_light_properties;

    ZTH_SSBO_FIELD(glm::vec3, point_light_position);
    LightProperties point_light_properties;
    LightAttenuation point_light_attenuation;

    ZTH_SSBO_FIELD(glm::vec3, spot_light_position);
    ZTH_SSBO_FIELD(glm::vec3, spot_light_direction);
    ZTH_SSBO_FIELD(GLfloat, spot_light_inner_cutoff);
    ZTH_SSBO_FIELD(GLfloat, spot_light_outer_cutoff);
    LightProperties spot_light_properties;
    LightAttenuation spot_light_attenuation;

    ZTH_SSBO_FIELD(GlslBool, has_directional_light = false);
    ZTH_SSBO_FIELD(GlslBool, has_point_light = false);
    ZTH_SSBO_FIELD(GlslBool, has_spot_light = false);
};

struct MaterialUboData
{
    ZTH_UBO_FIELD(glm::vec3, albedo);
    ZTH_UBO_FIELD(glm::vec3, ambient);
    ZTH_UBO_FIELD(glm::vec3, diffuse);
    ZTH_UBO_FIELD(glm::vec3, specular);
    ZTH_UBO_FIELD(GLfloat, shininess);

    ZTH_UBO_FIELD(GlslBool, has_diffuse_map = false);
    ZTH_UBO_FIELD(GlslBool, has_specular_map = false);
    ZTH_UBO_FIELD(GlslBool, has_emission_map = false);
};

} // namespace zth
