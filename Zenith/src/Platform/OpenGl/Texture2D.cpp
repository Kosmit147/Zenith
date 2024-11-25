#include "Zenith/Platform/OpenGl/Texture2D.hpp"

#include <stb_image/stb_image.h>

#include "Zenith/Logging/Logger.hpp"

namespace zth {

Texture2D::Texture2D(std::span<const u8> data, const TextureParams& params)
{
    stbi_set_flip_vertically_on_load(true);

    create();

    int width, height, channels;
    auto image = stbi_load_from_memory(data.data(), static_cast<int>(data.size_bytes()), &width, &height, &channels, 0);

    if (!image)
    {
        ZTH_CORE_ERROR("Failed to load texture from memory!");
        ZTH_DEBUG_BREAK;
        return;
    }

    glTextureParameteri(_id, GL_TEXTURE_WRAP_S, to_gl_int(params.horizontal_wrap));
    glTextureParameteri(_id, GL_TEXTURE_WRAP_T, to_gl_int(params.vertical_wrap));
    glTextureParameteri(_id, GL_TEXTURE_MIN_FILTER, to_gl_int(params.min_filter));
    glTextureParameteri(_id, GL_TEXTURE_MAG_FILTER, to_gl_int(params.mag_filter));

    glTextureStorage2D(_id, 1, to_gl_int(params.internal_format), width, height);

    auto format = texture_format_from_channels(channels);
    glTextureSubImage2D(_id, 0, 0, 0, width, height, to_gl_enum(format), GL_UNSIGNED_BYTE, image);
    glGenerateTextureMipmap(_id);

    stbi_image_free(image);
}

} // namespace zth
