#pragma once

#include <glad/glad.h>

#include <utility>

#include "Zenith/Core/Assert.hpp"
#include "Zenith/Core/Typedefs.hpp"

namespace zth {

enum class TextureFormat : u16
{
    R = GL_RED,
    Rg = GL_RG,
    Rgb = GL_RGB,
    Rgba = GL_RGBA,
};

enum class SizedTextureFormat : u16
{
    R8 = GL_R8,
    Rg8 = GL_RG8,
    Rgb8 = GL_RGB8,
    Rgba8 = GL_RGBA8,
};

enum class TextureWrapMode : u16
{
    Repeat = GL_REPEAT,
    MirroredRepeat = GL_MIRRORED_REPEAT,
    ClampToEdge = GL_CLAMP_TO_EDGE,
    ClampToBorder = GL_CLAMP_TO_BORDER,
};

enum class TextureFilteringMode : u8
{
    Nearest = 0,
    Linear = 1,
};

enum class MipmapFilteringMode : u8
{
    Nearest = 0,
    Linear = 2,
};

struct TextureMinFilter
{
    TextureFilteringMode texture_filtering;
    MipmapFilteringMode mipmap_filtering;

    static const TextureMinFilter nearest_mipmap_nearest;
    static const TextureMinFilter linear_mipmap_nearest;
    static const TextureMinFilter nearest_mipmap_linear;
    static const TextureMinFilter linear_mipmap_linear;
};

inline const TextureMinFilter TextureMinFilter::nearest_mipmap_nearest = { TextureFilteringMode::Nearest,
                                                                           MipmapFilteringMode::Nearest };
inline const TextureMinFilter TextureMinFilter::linear_mipmap_nearest = { TextureFilteringMode::Linear,
                                                                          MipmapFilteringMode::Nearest };
inline const TextureMinFilter TextureMinFilter::nearest_mipmap_linear = { TextureFilteringMode::Nearest,
                                                                          MipmapFilteringMode::Linear };
inline const TextureMinFilter TextureMinFilter::linear_mipmap_linear = { TextureFilteringMode::Linear,
                                                                         MipmapFilteringMode::Linear };
struct TextureMagFilter
{
    TextureFilteringMode texture_filtering;

    static const TextureMagFilter nearest;
    static const TextureMagFilter linear;
};

inline const TextureMagFilter TextureMagFilter::nearest = { TextureFilteringMode::Nearest };
inline const TextureMagFilter TextureMagFilter::linear = { TextureFilteringMode::Linear };

struct TextureParams
{
    TextureWrapMode horizontal_wrap = TextureWrapMode::Repeat;
    TextureWrapMode vertical_wrap = TextureWrapMode::Repeat;
    TextureMinFilter min_filter = TextureMinFilter::linear_mipmap_linear;
    TextureMagFilter mag_filter = TextureMagFilter::linear;
    SizedTextureFormat internal_format = SizedTextureFormat::Rgba8;
};

[[nodiscard]] constexpr auto to_gl_int(TextureWrapMode wrap) -> GLint
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

[[nodiscard]] constexpr auto to_gl_int(TextureMagFilter mag_filter) -> GLint
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

[[nodiscard]] constexpr auto to_gl_int(TextureMinFilter min_filter) -> GLint
{
    GLint res = GL_NEAREST_MIPMAP_NEAREST;
    res += std::to_underlying(min_filter.texture_filtering);
    res += std::to_underlying(min_filter.mipmap_filtering);
    return res;
}

[[nodiscard]] constexpr auto to_gl_int(SizedTextureFormat tex_format) -> GLint
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

[[nodiscard]] constexpr auto to_gl_enum(TextureFormat tex_format) -> GLenum
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

[[nodiscard]] constexpr auto texture_format_from_channels(u32 channels) -> TextureFormat
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

} // namespace zth
