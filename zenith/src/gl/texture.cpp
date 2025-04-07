#include "zenith/gl/texture.hpp"

#include <stb_image/stb_image.h>

#include "zenith/core/assert.hpp"
#include "zenith/fs/file.hpp"
#include "zenith/log/logger.hpp"
#include "zenith/memory/temporary_storage.hpp"
#include "zenith/util/defer.hpp"

namespace zth::gl {

constinit const TextureMinFilter TextureMinFilter::nearest_mipmap_nearest = { TextureFilteringMode::Nearest,
                                                                              MipmapFilteringMode::Nearest };
constinit const TextureMinFilter TextureMinFilter::linear_mipmap_nearest = { TextureFilteringMode::Linear,
                                                                             MipmapFilteringMode::Nearest };
constinit const TextureMinFilter TextureMinFilter::nearest_mipmap_linear = { TextureFilteringMode::Nearest,
                                                                             MipmapFilteringMode::Linear };
constinit const TextureMinFilter TextureMinFilter::linear_mipmap_linear = { TextureFilteringMode::Linear,
                                                                            MipmapFilteringMode::Linear };

constinit const TextureMagFilter TextureMagFilter::nearest = { TextureFilteringMode::Nearest };
constinit const TextureMagFilter TextureMagFilter::linear = { TextureFilteringMode::Linear };

Texture2D::Texture2D(const void* data, usize data_size_bytes, const TextureParams& params)
{
    init_from_memory(data, data_size_bytes, params);
}

Texture2D::Texture2D(const std::filesystem::path& path, const TextureParams& params)
{
    auto maybe_data = fs::load_to<TemporaryVector<u8>>(path);

    if (!maybe_data)
    {
        // @robustness: .string() throws.
        ZTH_CORE_ERROR("[Texture] Failed to load texture from file {}.", path.string());
        ZTH_DEBUG_BREAK;
        return;
    }

    std::span data{ *maybe_data };
    init_from_memory(data.data(), data.size_bytes(), params);
}

auto Texture2D::from_memory(const void* data, usize data_size_bytes, const TextureParams& params) -> Texture2D
{
    return Texture2D{ data, data_size_bytes, params };
}

auto Texture2D::from_file(const std::filesystem::path& path, const TextureParams& params) -> Texture2D
{
    return Texture2D{ path, params };
}

Texture2D::Texture2D(Texture2D&& other) noexcept : _id(std::exchange(other._id, GL_NONE)) {}

auto Texture2D::operator=(Texture2D&& other) noexcept -> Texture2D&
{
    _id = std::exchange(other._id, GL_NONE);
    return *this;
}

Texture2D::~Texture2D()
{
    destroy();
}

auto Texture2D::bind(u32 slot) const -> void
{
    glBindTextureUnit(slot, _id);
}

auto Texture2D::unbind(u32 slot) -> void
{
    glBindTextureUnit(slot, GL_NONE);
}

auto Texture2D::create() noexcept -> void
{
    glCreateTextures(GL_TEXTURE_2D, 1, &_id);
}

auto Texture2D::destroy() const noexcept -> void
{
    glDeleteTextures(1, &_id);
}

auto Texture2D::init_from_memory(const void* data, usize data_size_bytes, const TextureParams& params) -> void
{
    stbi_set_flip_vertically_on_load(true);
    Defer unset_flip_vertically_on_load{ [] { stbi_set_flip_vertically_on_load(false); } };

    int width, height, channels;
    auto image = stbi_load_from_memory(static_cast<const stbi_uc*>(data), static_cast<int>(data_size_bytes), &width,
                                       &height, &channels, 0);

    if (!image)
    {
        ZTH_CORE_ERROR("[Texture] Failed to load texture from memory.");
        ZTH_DEBUG_BREAK;
        return;
    }

    Defer free_image{ [&] { stbi_image_free(image); } };

    create();

    glTextureParameteri(_id, GL_TEXTURE_WRAP_S, to_gl_int(params.horizontal_wrap));
    glTextureParameteri(_id, GL_TEXTURE_WRAP_T, to_gl_int(params.vertical_wrap));
    glTextureParameteri(_id, GL_TEXTURE_MIN_FILTER, to_gl_int(params.min_filter));
    glTextureParameteri(_id, GL_TEXTURE_MAG_FILTER, to_gl_int(params.mag_filter));

    glTextureStorage2D(_id, 1, to_gl_enum(params.internal_format), width, height);

    auto format = texture_format_from_channels(static_cast<u32>(channels));
    glTextureSubImage2D(_id, 0, 0, 0, width, height, to_gl_enum(format), GL_UNSIGNED_BYTE, image);
    glGenerateTextureMipmap(_id);
}

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
