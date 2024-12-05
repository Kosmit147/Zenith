#version 460 core

layout (location = 0) in vec3 inLocalPosition;

uniform mat4 transform;
uniform mat4 viewProjection;

void main()
{
    gl_Position = viewProjection * transform * vec4(inLocalPosition, 1.0);
}
