#pragma once

#include <glad/glad.h>

#include <ranges>

#include "zenith/core/typedefs.hpp"
#include "zenith/util/macros.hpp"

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
    // @volatile: these numbers are chosen for converting TextureMinFilter to an OpenGL value

    Nearest = 0,
    Linear = 1,
};

enum class MipmapFilteringMode : u8
{
    // @volatile: these numbers are chosen for converting TextureMinFilter to an OpenGL value

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

struct TextureMagFilter
{
    TextureFilteringMode texture_filtering;

    static const TextureMagFilter nearest;
    static const TextureMagFilter linear;
};

struct TextureParams
{
    TextureWrapMode horizontal_wrap = TextureWrapMode::Repeat;
    TextureWrapMode vertical_wrap = TextureWrapMode::Repeat;
    TextureMinFilter min_filter = TextureMinFilter::linear_mipmap_linear;
    TextureMagFilter mag_filter = TextureMagFilter::linear;
    SizedTextureFormat internal_format = SizedTextureFormat::Rgba8;
};

class Texture2D
{
public:
    explicit Texture2D(const void* data, usize data_size_bytes, const TextureParams& params = {});
    explicit Texture2D(std::ranges::contiguous_range auto&& data, const TextureParams& params = {});

    ZTH_NO_COPY(Texture2D)

    Texture2D(Texture2D&& other) noexcept;
    auto operator=(Texture2D&& other) noexcept -> Texture2D&;

    ~Texture2D();

    auto bind(u32 slot = 0) const -> void;
    static auto unbind(u32 slot = 0) -> void;

    [[nodiscard]] auto native_handle() const { return _id; }

private:
    GLuint _id = GL_NONE;

private:
    auto create() -> void;
    auto destroy() const -> void;
};

[[nodiscard]] auto to_gl_int(TextureWrapMode wrap) -> GLint;
[[nodiscard]] auto to_gl_int(TextureMagFilter mag_filter) -> GLint;
[[nodiscard]] auto to_gl_int(TextureMinFilter min_filter) -> GLint;
[[nodiscard]] auto to_gl_enum(SizedTextureFormat tex_format) -> GLenum;
[[nodiscard]] auto to_gl_enum(TextureFormat tex_format) -> GLenum;
[[nodiscard]] auto texture_format_from_channels(u32 channels) -> TextureFormat;

Texture2D::Texture2D(std::ranges::contiguous_range auto&& data, const TextureParams& params)
    : Texture2D(data.data(), std::size(data) * sizeof(std::ranges::range_value_t<decltype(data)>), params)
{}

} // namespace zth::gl
