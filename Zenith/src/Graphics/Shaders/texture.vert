#version 460 core

layout (location = 0) in vec3 inLocalPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoords;

uniform mat4 transform;
uniform mat4 viewProjection;

out vec2 TexCoords;

void main()
{
    gl_Position = viewProjection * transform * vec4(inLocalPosition, 1.0);
    TexCoords = inTexCoords;
}
