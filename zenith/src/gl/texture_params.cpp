#include "zenith/gl/texture_params.hpp"

#include "zenith/core/assert.hpp"

namespace zth::gl {

auto to_gl_int(TextureWrapMode wrap) -> GLint
{
    switch (wrap)
    {
        using enum TextureWrapMode;
    case Repeat:
        return GL_REPEAT;
    case MirroredRepeat:
        return GL_MIRRORED_REPEAT;
    case ClampToEdge:
        return GL_CLAMP_TO_EDGE;
    case ClampToBorder:
        return GL_CLAMP_TO_BORDER;
    }

    ZTH_ASSERT(false);
    std::unreachable();
}

auto to_gl_int(TextureMagFilter mag_filter) -> GLint
{
    switch (mag_filter.texture_filtering)
    {
        using enum TextureFilteringMode;
    case Linear:
        return GL_LINEAR;
    case Nearest:
        return GL_NEAREST;
    }

    ZTH_ASSERT(false);
    std::unreachable();
}

auto to_gl_int(TextureMinFilter min_filter) -> GLint
{
    GLint res = GL_NEAREST_MIPMAP_NEAREST;
    res += std::to_underlying(min_filter.texture_filtering);
    res += std::to_underlying(min_filter.mipmap_filtering);
    return res;
}

auto to_gl_enum(SizedTextureFormat tex_format) -> GLenum
{
    switch (tex_format)
    {
        using enum SizedTextureFormat;
    case R8:
        return GL_R8;
    case Rg8:
        return GL_RG8;
    case Rgb8:
        return GL_RGB8;
    case Rgba8:
        return GL_RGBA8;
    }

    ZTH_ASSERT(false);
    std::unreachable();
}

auto to_gl_enum(TextureFormat tex_format) -> GLenum
{
    switch (tex_format)
    {
        using enum TextureFormat;
    case R:
        return GL_RED;
    case Rg:
        return GL_RG;
    case Rgb:
        return GL_RGB;
    case Rgba:
        return GL_RGBA;
    }

    ZTH_ASSERT(false);
    std::unreachable();
}

auto texture_format_from_channels(u32 channels) -> TextureFormat
{
    switch (channels)
    {
        using enum TextureFormat;
    case 1:
        return R;
    case 2:
        return Rg;
    case 3:
        return Rgb;
    case 4:
        return Rgba;
    }

    ZTH_ASSERT(false);
    std::unreachable();
}

} // namespace zth::gl
