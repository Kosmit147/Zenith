#version 460 core

layout (location = 0) in vec3 inLocalPosition;

layout (std140, binding = ZTH_CAMERA_MATRICES_UBO_BINDING_INDEX) uniform CameraMatrices
{
	mat4 viewProjection;
};

uniform mat4 transform;

void main()
{
    gl_Position = viewProjection * transform * vec4(inLocalPosition, 1.0);
}
