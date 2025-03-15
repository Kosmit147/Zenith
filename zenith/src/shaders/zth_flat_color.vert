#version 460 core

#include "zth_defines.glsl"

layout (location = 0) in vec3 in_local_position;

layout (location = 3) in vec3 in_transform_col_0;
layout (location = 4) in vec3 in_transform_col_1;
layout (location = 5) in vec3 in_transform_col_2;
layout (location = 6) in vec3 in_transform_col_3;

layout (std140, binding = ZTH_CAMERA_UBO_BINDING_POINT) uniform CameraUbo
{
    mat4 view_projection;
    vec3 position;
} camera;

void main()
{
    // Transform matrix's last row is always (0, 0, 0, 1).
    mat4 transform = mat4(
        vec4(in_transform_col_0, 0.0),
        vec4(in_transform_col_1, 0.0),
        vec4(in_transform_col_2, 0.0),
        vec4(in_transform_col_3, 1.0)
    );

    gl_Position = camera.view_projection * transform * vec4(in_local_position, 1.0);
}
