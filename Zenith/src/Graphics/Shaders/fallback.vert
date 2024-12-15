#version 460 core

layout (location = 0) in vec3 inLocalPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoords;

layout (location = 3) in vec4 inTransformRow0;
layout (location = 4) in vec4 inTransformRow1;
layout (location = 5) in vec4 inTransformRow2;

layout (std140, binding = ZTH_CAMERA_MATRICES_UBO_BINDING_INDEX) uniform CameraMatrices
{
	mat4 viewProjection;
};

void main()
{
    mat4 transform = transpose(mat4(inTransformRow0, inTransformRow1, inTransformRow2, vec4(0.0, 0.0, 0.0, 1.0)));
    gl_Position = viewProjection * transform * vec4(inLocalPosition, 1.0);
}
