#pragma once

#include <glad/glad.h>

#include <ranges>

#include "Zenith/Core/Typedefs.hpp"
#include "Zenith/Platform/OpenGl/TextureParams.hpp"
#include "Zenith/Utility/Macros.hpp"

namespace zth {

class Texture
{
public:
    virtual ~Texture() = default;

    virtual auto bind(u32 slot = 0) const -> void = 0;
    virtual auto unbind(u32 slot = 0) const -> void = 0;
};

class Texture2D : public Texture
{
public:
    explicit Texture2D(std::ranges::contiguous_range auto&& data, const TextureParams& params = {});

    ZTH_NO_COPY(Texture2D)

    Texture2D(Texture2D&& other) noexcept;
    auto operator=(Texture2D&& other) noexcept -> Texture2D&;

    ~Texture2D() override;

    auto bind(u32 slot = 0) const -> void override;
    auto unbind(u32 slot = 0) const -> void override;

    [[nodiscard]] auto native_handle() const { return _id; }

private:
    GLuint _id = GL_NONE;

private:
    auto create() -> void;
    auto destroy() const -> void;
};

} // namespace zth

#include "Texture.inl"
