#pragma once

#include <glad/glad.h>

#include <array>
#include <span>
#include <vector>

#include "Zenith/Core/Typedefs.hpp"
#include "Zenith/Platform/OpenGl/TextureParams.hpp"
#include "Zenith/Utility/Utility.hpp"

namespace zth {

class Texture2D
{
public:
    explicit Texture2D(std::span<const u8> data, const TextureParams& params = {});

    template<usize DataSize>
    explicit Texture2D(std::span<const u8, DataSize> data, const TextureParams& params = {})
        : Texture2D(std::span<const u8, std::dynamic_extent>{ data }, params)
    {}

    template<usize DataSize>
    explicit Texture2D(const std::array<u8, DataSize>& data, const TextureParams& params = {})
        : Texture2D(std::span{ data }, params)
    {}

    explicit Texture2D(const std::vector<u8>& data, const TextureParams& params = {})
        : Texture2D(std::span{ data }, params)
    {}

    ZTH_NO_COPY_NO_MOVE(Texture2D)
    ~Texture2D() { destroy(); }

    auto bind(u32 slot = 0) const -> void { glBindTextureUnit(slot, _id); }
    auto unbind(u32 slot = 0) const -> void { glBindTextureUnit(slot, 0); }

    [[nodiscard]] auto native_handle() const { return _id; }

private:
    GLuint _id = GL_NONE;

private:
    auto create() -> void { glCreateTextures(GL_TEXTURE_2D, 1, &_id); }
    auto destroy() const -> void { glDeleteTextures(1, &_id); }
};

} // namespace zth
