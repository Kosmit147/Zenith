#version 460 core

layout (location = 0) in vec3 inLocalPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;

layout (location = 3) in vec4 inTransformRow0;
layout (location = 4) in vec4 inTransformRow1;
layout (location = 5) in vec4 inTransformRow2;

layout (location = 6) in vec3 inNormalMatCol0;
layout (location = 7) in vec3 inNormalMatCol1;
layout (location = 8) in vec3 inNormalMatCol2;

layout (std140, binding = ZTH_CAMERA_UBO_BINDING_INDEX) uniform CameraUbo
{
	mat4 viewProjection;
    vec3 cameraPosition;
};

void main()
{
    mat4 transform = transpose(mat4(inTransformRow0, inTransformRow1, inTransformRow2, vec4(0.0, 0.0, 0.0, 1.0)));
    gl_Position = viewProjection * transform * vec4(inLocalPosition, 1.0);
}
