#version 460 core

in vec4 Color;
in vec2 TexCoords;

uniform sampler2D tex;

out vec4 outColor;

void main()
{
    outColor = texture(tex, TexCoords) * Color;
}
