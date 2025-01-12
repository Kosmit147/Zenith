#pragma once

#include <glad/glad.h>

#include <ranges>

#include "Zenith/Core/Typedefs.hpp"
#include "Zenith/Platform/OpenGl/TextureParams.hpp"
#include "Zenith/Utility/Macros.hpp"

namespace zth {

class Texture2D
{
public:
    explicit Texture2D(std::ranges::contiguous_range auto&& data, const TextureParams& params = {});

    ZTH_NO_COPY(Texture2D)

    Texture2D(Texture2D&& other) noexcept;
    auto operator=(Texture2D&& other) noexcept -> Texture2D&;

    ~Texture2D();

    auto bind(u32 slot = 0) const -> void;
    auto unbind(u32 slot = 0) const -> void;

    [[nodiscard]] auto native_handle() const { return _id; }

private:
    GLuint _id = GL_NONE;

private:
    auto create() -> void;
    auto destroy() const -> void;
};

} // namespace zth

#include "Texture.inl"
