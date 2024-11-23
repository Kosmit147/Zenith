#version 460 core

#define PI 3.14159265359 

layout (location = 0) in vec4 inColor;

uniform float time;

out vec4 Color;

void main()
{
    float offset = (2.0 * PI / 3.0) * gl_VertexID;
    vec2 pos = vec2(sin(time + offset) * 0.5, cos(time + offset) * 0.5);

    gl_Position = vec4(pos, 0.0, 1.0);
    Color = inColor;
}
