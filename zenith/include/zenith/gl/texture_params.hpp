#pragma once

#include <glad/glad.h>

#include "zenith/core/typedefs.hpp"

namespace zth::gl {

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
    // @volatile: these numbers are chosen to make converting TextureMinFilter to an OpenGL value easier

    Nearest = 0,
    Linear = 1,
};

enum class MipmapFilteringMode : u8
{
    // @volatile: these numbers are chosen to make converting TextureMinFilter to an OpenGL value easier

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

[[nodiscard]] auto to_gl_int(TextureWrapMode wrap) -> GLint;
[[nodiscard]] auto to_gl_int(TextureMagFilter mag_filter) -> GLint;
[[nodiscard]] auto to_gl_int(TextureMinFilter min_filter) -> GLint;
[[nodiscard]] auto to_gl_enum(SizedTextureFormat tex_format) -> GLenum;
[[nodiscard]] auto to_gl_enum(TextureFormat tex_format) -> GLenum;
[[nodiscard]] auto texture_format_from_channels(u32 channels) -> TextureFormat;

} // namespace zth::gl
