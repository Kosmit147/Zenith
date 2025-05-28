#include "zenith/gl/texture.hpp"

#include <stb_image/stb_image.h>

#include "zenith/core/assert.hpp"
#include "zenith/log/logger.hpp"
#include "zenith/memory/buffer.hpp"
#include "zenith/system/file.hpp"
#include "zenith/system/temporary_storage.hpp"
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

auto Texture2D::from_rgb(std::span<const float> data, u32 width, u32 height, const TextureParams& params) -> Texture2D
{
    return Texture2D{ FromRgbTag{}, data, width, height, params };
}

auto Texture2D::from_rgba(std::span<const float> data, u32 width, u32 height, const TextureParams& params) -> Texture2D
{

    return Texture2D{ FromRgbaTag{}, data, width, height, params };
}

auto Texture2D::from_rgb(std::span<const glm::vec3> data, u32 width, u32 height, const TextureParams& params)
    -> Texture2D
{
    return Texture2D{ FromRgbTag{}, data, width, height, params };
}

auto Texture2D::from_rgba(std::span<const glm::vec4> data, u32 width, u32 height, const TextureParams& params)
    -> Texture2D
{
    return Texture2D{ FromRgbaTag{}, data, width, height, params };
}

auto Texture2D::from_rgb8(std::span<const u8> data, u32 width, u32 height, const TextureParams& params) -> Texture2D
{
    return Texture2D{ FromRgb8Tag{}, data, width, height, params };
}

auto Texture2D::from_rgba8(std::span<const u8> data, u32 width, u32 height, const TextureParams& params) -> Texture2D
{
    return Texture2D{ FromRgba8Tag{}, data, width, height, params };
}

auto Texture2D::from_rgb8(std::span<const glm::vec<3, u8>> data, u32 width, u32 height, const TextureParams& params)
    -> Texture2D
{
    return Texture2D{ FromRgb8Tag{}, data, width, height, params };
}

auto Texture2D::from_rgba8(std::span<const glm::vec<4, u8>> data, u32 width, u32 height, const TextureParams& params)
    -> Texture2D
{
    return Texture2D{ FromRgba8Tag{}, data, width, height, params };
}

auto Texture2D::from_file(const std::filesystem::path& path, const TextureParams& params) -> Texture2D
{
    return Texture2D{ FromFileTag{}, path, params };
}

auto Texture2D::from_file_data(std::span<const byte> file_data, const TextureParams& params) -> Texture2D
{
    return Texture2D{ FromFileDataTag{}, file_data, params };
}

Texture2D::Texture2D(Texture2D&& other) noexcept : _id{ std::exchange(other._id, GL_NONE) } {}

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

Texture2D::Texture2D(FromRgbTag, std::span<const float> data, u32 width, u32 height, const TextureParams& params)
{
    create_from_pixels(std::as_bytes(data), DataType::Float, width, height, TextureFormat::Rgb, params);
}

Texture2D::Texture2D(FromRgbaTag, std::span<const float> data, u32 width, u32 height, const TextureParams& params)
{
    create_from_pixels(std::as_bytes(data), DataType::Float, width, height, TextureFormat::Rgba, params);
}

Texture2D::Texture2D(FromRgbTag, std::span<const glm::vec3> data, u32 width, u32 height, const TextureParams& params)
{
    create_from_pixels(std::as_bytes(data), DataType::Float, width, height, TextureFormat::Rgb, params);
}

Texture2D::Texture2D(FromRgbaTag, std::span<const glm::vec4> data, u32 width, u32 height, const TextureParams& params)
{
    create_from_pixels(std::as_bytes(data), DataType::Float, width, height, TextureFormat::Rgba, params);
}

Texture2D::Texture2D(FromRgb8Tag, std::span<const u8> data, u32 width, u32 height, const TextureParams& params)
{
    create_from_pixels(std::as_bytes(data), DataType::UnsignedByte, width, height, TextureFormat::Rgb, params);
}

Texture2D::Texture2D(FromRgba8Tag, std::span<const u8> data, u32 width, u32 height, const TextureParams& params)
{
    create_from_pixels(std::as_bytes(data), DataType::UnsignedByte, width, height, TextureFormat::Rgba, params);
}

Texture2D::Texture2D(FromRgb8Tag, std::span<const glm::vec<3, u8>> data, u32 width, u32 height,
                     const TextureParams& params)
{
    create_from_pixels(std::as_bytes(data), DataType::UnsignedByte, width, height, TextureFormat::Rgb, params);
}

Texture2D::Texture2D(FromRgba8Tag, std::span<const glm::vec<4, u8>> data, u32 width, u32 height,
                     const TextureParams& params)
{
    create_from_pixels(std::as_bytes(data), DataType::UnsignedByte, width, height, TextureFormat::Rgba, params);
}

Texture2D::Texture2D(FromFileTag, const std::filesystem::path& path, const TextureParams& params)
{
    auto file_data = fs::read_to<TemporaryBuffer>(path);

    if (!file_data)
    {
        // @robustness: .string() throws.
        ZTH_INTERNAL_ERROR("[Texture] Failed to load texture from file \"{}\".", path.string());
        ZTH_DEBUG_BREAK;
        return;
    }

    create_from_file_data(*file_data, params);
}

Texture2D::Texture2D(FromFileDataTag, std::span<const byte> file_data, const TextureParams& params)
{
    create_from_file_data(file_data, params);
}

auto Texture2D::create() noexcept -> void
{
    glCreateTextures(GL_TEXTURE_2D, 1, &_id);
}

auto Texture2D::destroy() const noexcept -> void
{
    glDeleteTextures(1, &_id);
}

auto Texture2D::create_from_file_data(std::span<const byte> file_data, const TextureParams& params) -> void
{
    stbi_set_flip_vertically_on_load(true);
    Defer unset_flip_vertically_on_load{ [] { stbi_set_flip_vertically_on_load(false); } };

    int width, height, channels;
    auto image = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(file_data.data()),
                                       static_cast<int>(file_data.size_bytes()), &width, &height, &channels, 0);

    if (!image)
    {
        ZTH_INTERNAL_ERROR("[Texture] Failed to load texture from memory.");
        ZTH_DEBUG_BREAK;
        return;
    }

    auto image_data = std::as_bytes(
        std::span{ image, static_cast<usize>(width) * static_cast<usize>(height) * static_cast<usize>(channels) });
    create_from_pixels(image_data, DataType::UnsignedByte, width, height,
                       texture_format_from_channels(static_cast<u32>(channels)), params);
    stbi_image_free(image);
}

auto Texture2D::create_from_pixels(std::span<const byte> pixels, DataType type, u32 width, u32 height,
                                   TextureFormat format, const TextureParams& params) noexcept -> void
{
#if defined(ZTH_ASSERTIONS)
    {
        auto pixel_count = static_cast<usize>(width) * static_cast<usize>(height);
        auto bytes_per_pixel = static_cast<usize>(channels_in_texture_format(format)) * size_of_data_type(type);

        ZTH_ASSERT(pixels.size_bytes() == pixel_count * bytes_per_pixel);
    }
#endif

    create();

    glTextureParameteri(_id, GL_TEXTURE_WRAP_S, to_gl_int(params.horizontal_wrap));
    glTextureParameteri(_id, GL_TEXTURE_WRAP_T, to_gl_int(params.vertical_wrap));
    glTextureParameteri(_id, GL_TEXTURE_MIN_FILTER, to_gl_int(params.min_filter));
    glTextureParameteri(_id, GL_TEXTURE_MAG_FILTER, to_gl_int(params.mag_filter));

    glTextureStorage2D(_id, 1, to_gl_enum(params.internal_format), static_cast<GLsizei>(width),
                       static_cast<GLsizei>(height));
    glTextureSubImage2D(_id, 0, 0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height), to_gl_enum(format),
                        to_gl_enum(type), pixels.data());
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

auto to_gl_enum(SizedTextureFormat format) -> GLenum
{
    switch (format)
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

auto to_gl_enum(TextureFormat format) -> GLenum
{
    switch (format)
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

auto channels_in_texture_format(TextureFormat format) -> u32
{
    switch (format)
    {
        using enum TextureFormat;
    case R:
        return 1;
    case Rg:
        return 2;
    case Rgb:
        return 3;
    case Rgba:
        return 4;
    }

    ZTH_ASSERT(false);
    return 0;
}

} // namespace zth::gl
