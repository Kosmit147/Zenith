#version 460 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexCoords;
layout (location = 2) in vec4 inColor;

uniform mat4 transform;
uniform mat4 view_projection;

out vec2 TexCoords;
out vec4 Color;

void main()
{
    gl_Position = view_projection * transform * vec4(inPosition, 1.0);

    TexCoords = inTexCoords;
    Color = inColor;
}
