#pragma once

#include <stb_image/stb_image.h>

#include "Zenith/Logging/Logger.hpp"

namespace zth {

Texture2D::Texture2D(std::ranges::contiguous_range auto&& data, const TextureParams& params)
{
    using DataType = std::ranges::range_value_t<decltype(data)>;
    const auto data_size_bytes = data.size() * sizeof(DataType);

    stbi_set_flip_vertically_on_load(true);

    int width, height, channels;
    auto image = stbi_load_from_memory(data.data(), static_cast<int>(data_size_bytes), &width, &height, &channels, 0);

    if (!image) [[unlikely]]
    {
        ZTH_CORE_ERROR("Failed to load texture from memory!");
        ZTH_DEBUG_BREAK;
        return;
    }

    create();

    glTextureParameteri(_id, GL_TEXTURE_WRAP_S, to_gl_int(params.horizontal_wrap));
    glTextureParameteri(_id, GL_TEXTURE_WRAP_T, to_gl_int(params.vertical_wrap));
    glTextureParameteri(_id, GL_TEXTURE_MIN_FILTER, to_gl_int(params.min_filter));
    glTextureParameteri(_id, GL_TEXTURE_MAG_FILTER, to_gl_int(params.mag_filter));

    glTextureStorage2D(_id, 1, to_gl_enum(params.internal_format), width, height);

    auto format = texture_format_from_channels(static_cast<u32>(channels));
    glTextureSubImage2D(_id, 0, 0, 0, width, height, to_gl_enum(format), GL_UNSIGNED_BYTE, image);
    glGenerateTextureMipmap(_id);

    stbi_image_free(image);
    stbi_set_flip_vertically_on_load(false);
}

} // namespace zth
