#version 460 core

layout (location = 0) in vec3 inLocalPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoords;

layout (std140, binding = ZTH_CAMERA_MATRICES_UBO_BINDING_INDEX) uniform CameraMatrices
{
	mat4 viewProjection;
};

uniform mat4 transform;

out vec2 TexCoords;

void main()
{
    gl_Position = viewProjection * transform * vec4(inLocalPosition, 1.0);
    TexCoords = inTexCoords;
}
