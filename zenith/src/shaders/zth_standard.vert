#version 460 core

#include "zth_defines.glsl"

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_uv;

layout (location = 3) in vec3 in_transform_col_0;
layout (location = 4) in vec3 in_transform_col_1;
layout (location = 5) in vec3 in_transform_col_2;
layout (location = 6) in vec3 in_transform_col_3;

layout (location = 7) in mat3 in_normal_mat;

layout (std140, binding = ZTH_CAMERA_UBO_BINDING_POINT) uniform CameraUbo
{
	mat4 view_projection;
    vec3 position;
} camera;

out vec3 Position;
out flat vec3 Normal;
out vec2 UV;

void main()
{
    // Transform matrix's last row is always (0, 0, 0, 1).
    mat4 transform = mat4(
        vec4(in_transform_col_0, 0.0),
        vec4(in_transform_col_1, 0.0),
        vec4(in_transform_col_2, 0.0),
        vec4(in_transform_col_3, 1.0)
    );

    vec4 world_position = transform * vec4(in_position, 1.0);

    Position = world_position.xyz;
    Normal = normalize(in_normal_mat * in_normal);
    UV = in_uv;

    gl_Position = camera.view_projection * world_position;
}
