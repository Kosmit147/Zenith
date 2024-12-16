#version 460 core

layout (std140, binding = ZTH_MATERIAL_UBO_BINDING_INDEX) uniform MaterialUbo
{
	vec3 albedo;
};

out vec4 outColor;

void main()
{
    outColor = vec4(albedo, 1.0);
}
