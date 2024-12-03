#version 460 core

in vec2 TexCoords;
in vec4 Color;

uniform sampler2D tex;

out vec4 outColor;

void main()
{
    outColor = texture(tex, TexCoords) * Color;
}
