#version 460 core

#include "zth_defines.glsl"

in vec2 UV;
in vec4 Color;

layout (binding = ZTH_TEXTURE_2D_SLOT) uniform sampler2D tex;

out vec4 out_color;

void main()
{
    out_color = Color * texture(tex, UV);
}
