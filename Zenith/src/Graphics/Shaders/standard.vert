#version 460 core

layout (location = 0) in vec3 inLocalPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;

layout (location = 3) in vec4 inTransformRow0;
layout (location = 4) in vec4 inTransformRow1;
layout (location = 5) in vec4 inTransformRow2;

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
    mat4 transform = transpose(mat4(inTransformRow0, inTransformRow1, inTransformRow2, vec4(0.0, 0.0, 0.0, 1.0)));

    vec4 worldPosition = transform * vec4(inLocalPosition, 1.0);
    Position = worldPosition.xyz;

    // TODO: calculate normal matrix on cpu side
    mat3 normalMatrix = mat3(transpose(inverse(transform)));
    Normal = normalize(normalMatrix * inNormal);

    UV = inUV;

    gl_Position = viewProjection * worldPosition;
}
