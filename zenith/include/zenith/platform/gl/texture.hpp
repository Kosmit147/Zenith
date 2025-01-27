#pragma once

#include <glad/glad.h>

#include <ranges>

#include "zenith/core/typedefs.hpp"
#include "zenith/platform/gl/texture_params.hpp"
#include "zenith/util/macros.hpp"

namespace zth {

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

Texture2D::Texture2D(std::ranges::contiguous_range auto&& data, const TextureParams& params)
    : Texture2D(data.data(), std::size(data) * sizeof(std::ranges::range_value_t<decltype(data)>), params)
{}

} // namespace zth
