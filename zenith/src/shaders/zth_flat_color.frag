#version 460 core

#include "zth_defines.glsl"

layout (std140, binding = ZTH_MATERIAL_UBO_BINDING_INDEX) uniform MaterialUbo
{
	vec3 albedo;
};

out vec4 out_color;

void main()
{
    out_color = vec4(albedo, 1.0);
}
