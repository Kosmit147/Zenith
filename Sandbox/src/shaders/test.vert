#version 460 core

#define PI 3.14159265359 

layout (location = 0) in vec4 inColor;
layout (location = 1) in vec2 inTexCoords;

uniform float time;
uniform mat4 transform;
uniform mat4 view_projection;

out vec4 Color;
out vec2 TexCoords;

void main()
{
    float offset = (2.0 * PI / 3.0) * gl_VertexID;
    vec2 pos = vec2(sin(time + offset) * 0.7, cos(time + offset) * 0.7);

    gl_Position = view_projection * transform * vec4(pos, 0.0, 1.0);

    Color = inColor;
    TexCoords = inTexCoords;
}
