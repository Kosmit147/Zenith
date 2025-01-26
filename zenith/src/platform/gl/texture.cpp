#include "zenith/platform/gl/texture.hpp"

#include <stb_image/stb_image.h>

#include "zenith/logging/logger.hpp"
#include "zenith/utility/cleanup.hpp"

namespace zth {

Texture2D::Texture2D(const void* data, usize data_size_bytes, const TextureParams& params)
{
    stbi_set_flip_vertically_on_load(true);
    Cleanup unset_flip_vertically_on_load{ [] { stbi_set_flip_vertically_on_load(false); } };

    int width, height, channels;
    auto image = stbi_load_from_memory(static_cast<const stbi_uc*>(data), static_cast<int>(data_size_bytes),
                                       &width, &height, &channels, 0);

    if (!image)
    {
        ZTH_CORE_ERROR("Failed to load texture from memory.");
        ZTH_DEBUG_BREAK;
        return;
    }

    Cleanup image_cleanup{ [&] { stbi_image_free(image); } };

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

Texture2D::Texture2D(Texture2D&& other) noexcept : _id(other._id)
{
    other._id = GL_NONE;
}

auto Texture2D::operator=(Texture2D&& other) noexcept -> Texture2D&
{
    _id = other._id;
    other._id = GL_NONE;
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

auto Texture2D::create() -> void
{
    glCreateTextures(GL_TEXTURE_2D, 1, &_id);
}

auto Texture2D::destroy() const -> void
{
    glDeleteTextures(1, &_id);
}

} // namespace zth
