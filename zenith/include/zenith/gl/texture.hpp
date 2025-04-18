#pragma once

#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <filesystem>
#include <span>

#include "zenith/core/typedefs.hpp"
#include "zenith/gl/util.hpp"
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
    // @volatile: These numbers are chosen for converting TextureMinFilter to an OpenGL value.

    Nearest = 0,
    Linear = 1,
};

enum class MipmapFilteringMode : u8
{
    // @volatile: These numbers are chosen for converting TextureMinFilter to an OpenGL value.

    Nearest = 0,
    Linear = 2,
};

struct TextureMinFilter
{
    TextureFilteringMode texture_filtering;
    MipmapFilteringMode mipmap_filtering;

    static constinit const TextureMinFilter nearest_mipmap_nearest;
    static constinit const TextureMinFilter linear_mipmap_nearest;
    static constinit const TextureMinFilter nearest_mipmap_linear;
    static constinit const TextureMinFilter linear_mipmap_linear;
};

struct TextureMagFilter
{
    TextureFilteringMode texture_filtering;

    static constinit const TextureMagFilter nearest;
    static constinit const TextureMagFilter linear;
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
    using TextureId = GLuint;

    [[nodiscard]] static auto from_rgb(std::span<const float> data, u32 width, u32 height,
                                       const TextureParams& params = {}) -> Texture2D;
    [[nodiscard]] static auto from_rgba(std::span<const float> data, u32 width, u32 height,
                                        const TextureParams& params = {}) -> Texture2D;
    [[nodiscard]] static auto from_rgb(std::span<const glm::vec3> data, u32 width, u32 height,
                                       const TextureParams& params = {}) -> Texture2D;
    [[nodiscard]] static auto from_rgba(std::span<const glm::vec4> data, u32 width, u32 height,
                                        const TextureParams& params = {}) -> Texture2D;

    [[nodiscard]] static auto from_rgb8(std::span<const u8> data, u32 width, u32 height,
                                        const TextureParams& params = {}) -> Texture2D;
    [[nodiscard]] static auto from_rgba8(std::span<const u8> data, u32 width, u32 height,
                                         const TextureParams& params = {}) -> Texture2D;
    [[nodiscard]] static auto from_rgb8(std::span<const glm::vec<3, u8>> data, u32 width, u32 height,
                                        const TextureParams& params = {}) -> Texture2D;
    [[nodiscard]] static auto from_rgba8(std::span<const glm::vec<4, u8>> data, u32 width, u32 height,
                                         const TextureParams& params = {}) -> Texture2D;

    [[nodiscard]] static auto from_file(const std::filesystem::path& path, const TextureParams& params = {})
        -> Texture2D;
    [[nodiscard]] static auto from_file_data(std::span<const byte> file_data, const TextureParams& params = {})
        -> Texture2D;

    ZTH_NO_COPY(Texture2D)

    Texture2D(Texture2D&& other) noexcept;
    auto operator=(Texture2D&& other) noexcept -> Texture2D&;

    ~Texture2D();

    auto bind(u32 slot = 0) const -> void;
    static auto unbind(u32 slot = 0) -> void;

    [[nodiscard]] auto native_handle() const { return _id; }

private:
    // clang-format off

    struct FromRgbTag {};
    struct FromRgbaTag {};
    struct FromRgb8Tag {};
    struct FromRgba8Tag {};

    struct FromFileTag {};
    struct FromFileDataTag {};

    // clang-format on

    TextureId _id = GL_NONE;

private:
    explicit Texture2D(FromRgbTag, std::span<const float> data, u32 width, u32 height, const TextureParams& params);
    explicit Texture2D(FromRgbaTag, std::span<const float> data, u32 width, u32 height, const TextureParams& params);
    explicit Texture2D(FromRgbTag, std::span<const glm::vec3> data, u32 width, u32 height, const TextureParams& params);
    explicit Texture2D(FromRgbaTag, std::span<const glm::vec4> data, u32 width, u32 height,
                       const TextureParams& params);

    explicit Texture2D(FromRgb8Tag, std::span<const u8> data, u32 width, u32 height, const TextureParams& params);
    explicit Texture2D(FromRgba8Tag, std::span<const u8> data, u32 width, u32 height, const TextureParams& params);
    explicit Texture2D(FromRgb8Tag, std::span<const glm::vec<3, u8>> data, u32 width, u32 height,
                       const TextureParams& params);
    explicit Texture2D(FromRgba8Tag, std::span<const glm::vec<4, u8>> data, u32 width, u32 height,
                       const TextureParams& params);

    explicit Texture2D(FromFileTag, const std::filesystem::path& path, const TextureParams& params);
    explicit Texture2D(FromFileDataTag, std::span<const byte> file_data, const TextureParams& params);

    auto create() noexcept -> void;
    auto destroy() const noexcept -> void;

    auto create_from_file_data(std::span<const byte> file_data, const TextureParams& params) -> void;
    auto create_from_pixels(std::span<const byte> pixels, DataType type, u32 width, u32 height, TextureFormat format,
                            const TextureParams& params) noexcept -> void;
};

[[nodiscard]] auto to_gl_int(TextureWrapMode wrap) -> GLint;
[[nodiscard]] auto to_gl_int(TextureMagFilter mag_filter) -> GLint;
[[nodiscard]] auto to_gl_int(TextureMinFilter min_filter) -> GLint;
[[nodiscard]] auto to_gl_enum(SizedTextureFormat format) -> GLenum;
[[nodiscard]] auto to_gl_enum(TextureFormat format) -> GLenum;
[[nodiscard]] auto texture_format_from_channels(u32 channels) -> TextureFormat;
[[nodiscard]] auto channels_in_texture_format(TextureFormat format) -> u32;

} // namespace zth::gl
