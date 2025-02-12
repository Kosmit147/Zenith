#version 460 core

#include "zth_defines.glsl"

layout (std140, binding = ZTH_MATERIAL_UBO_BINDING_POINT) uniform MaterialUbo
{
    vec3 albedo;
} material;

out vec4 out_color;

void main()
{
    out_color = vec4(material.albedo, 1.0);
}
