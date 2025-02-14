#pragma once

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "zenith/core/typedefs.hpp"
#include "zenith/gl/util.hpp"
#include "zenith/util/macros.hpp"

namespace zth {

#define ZTH_UBO_FIELD(type, name) alignas(::zth::gl::get_std140_field_alignment<type>()) type name
#define ZTH_SSBO_FIELD(type, name) alignas(::zth::gl::get_std430_field_alignment<type>()) type name
#define ZTH_PAD(bytes)                                                                                                 \
    u8 ZTH_UNIQUE_NAME(pad)[bytes] {}

using GlslBool = gl::GlslBool;

struct LightPropertiesData
{
    ZTH_SSBO_FIELD(glm::vec3, color);
    ZTH_SSBO_FIELD(glm::vec3, ambient);
    ZTH_SSBO_FIELD(glm::vec3, diffuse);
    ZTH_SSBO_FIELD(glm::vec3, specular);
};

struct LightAttenuationData
{
    ZTH_SSBO_FIELD(GLfloat, constant);
    ZTH_SSBO_FIELD(GLfloat, linear);
    ZTH_SSBO_FIELD(GLfloat, quadratic);
};

struct CameraUboData
{
    ZTH_UBO_FIELD(glm::mat4, view_projection);
    ZTH_UBO_FIELD(glm::vec3, camera_position);
};

struct DirectionalLightData
{
    ZTH_SSBO_FIELD(glm::vec3, direction);
    ZTH_SSBO_FIELD(LightPropertiesData, properties);
};

struct DirectionalLightsSsboData
{
    ZTH_SSBO_FIELD(GLuint, count = 0);
    ZTH_PAD(12);
    // here goes a variable-length array of DirectionalLightData (aligned at 16 bytes)
};

struct PointLightData
{
    ZTH_SSBO_FIELD(glm::vec3, position);
    ZTH_SSBO_FIELD(LightPropertiesData, properties);
    ZTH_SSBO_FIELD(LightAttenuationData, attenuation);
};

struct PointLightsSsboData
{
    ZTH_SSBO_FIELD(GLuint, count = 0);
    ZTH_PAD(12);
    // here goes a variable-length array of PointLightData (aligned at 16 bytes)
};

struct SpotLightData
{
    ZTH_SSBO_FIELD(glm::vec3, position);
    ZTH_SSBO_FIELD(glm::vec3, direction);
    ZTH_SSBO_FIELD(GLfloat, inner_cutoff);
    ZTH_SSBO_FIELD(GLfloat, outer_cutoff);
    ZTH_SSBO_FIELD(LightPropertiesData, properties);
    ZTH_SSBO_FIELD(LightAttenuationData, attenuation);
};

struct SpotLightsSsboData
{
    ZTH_SSBO_FIELD(GLuint, count = 0);
    ZTH_PAD(12);
    // here goes a variable-length array of SpotLightData (aligned at 16 bytes)
};

struct AmbientLightData
{
    ZTH_SSBO_FIELD(glm::vec3, ambient);
};

struct AmbientLightsSsboData
{
    ZTH_SSBO_FIELD(GLuint, count = 0);
    ZTH_PAD(12);
    // here goes a variable-length array of AmbientLightData (aligned at 16 bytes)
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
