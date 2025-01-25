#version 460 core

#include "zth_defines.glsl"

layout (location = 0) in vec3 inLocalPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;

layout (location = 3) in vec3 inTransformCol0;
layout (location = 4) in vec3 inTransformCol1;
layout (location = 5) in vec3 inTransformCol2;
layout (location = 6) in vec3 inTransformCol3;

layout (location = 7) in mat3 inNormalMat;

layout (std140, binding = ZTH_CAMERA_UBO_BINDING_INDEX) uniform CameraUbo
{
	mat4 viewProjection;
    vec3 cameraPosition;
};

out vec3 Position;
out flat vec3 Normal;
out vec2 UV;

void main()
{
    // transform matrix's last row is always (0, 0, 0, 1)
    mat4 transform = mat4(
        vec4(inTransformCol0, 0.0),
        vec4(inTransformCol1, 0.0),
        vec4(inTransformCol2, 0.0),
        vec4(inTransformCol3, 1.0)
    );

    vec4 worldPosition = transform * vec4(inLocalPosition, 1.0);

    Position = worldPosition.xyz;
    Normal = normalize(inNormalMat * inNormal);
    UV = inUV;

    gl_Position = viewProjection * worldPosition;
}
