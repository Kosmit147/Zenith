#pragma once

#include <glad/glad.h>

#include <ranges>

#include "Zenith/Platform/OpenGl/BufferUsage.hpp"
#include "Zenith/Platform/OpenGl/VertexLayout.hpp"
#include "Zenith/Utility/Utility.hpp"

namespace zth {

class GlBuffer
{
public:
    explicit GlBuffer();
    explicit GlBuffer(std::ranges::contiguous_range auto&& data, BufferUsage usage);

    ZTH_NO_COPY(GlBuffer)

    GlBuffer(GlBuffer&& other) noexcept;
    auto operator=(GlBuffer&& other) noexcept -> GlBuffer&;

    virtual ~GlBuffer();

    auto buffer_data(std::ranges::contiguous_range auto&& data, BufferUsage usage) -> void;

    virtual auto bind() const -> void = 0;
    virtual auto unbind() const -> void = 0;

    virtual auto free() -> void;

    [[nodiscard]] auto native_handle() const { return _id; }
    [[nodiscard]] auto size() const { return _size; }
    [[nodiscard]] auto size_bytes() const { return _size_bytes; }

private:
    GLuint _id = GL_NONE;
    GLsizei _size = 0;
    GLsizei _size_bytes = 0;

private:
    auto create() -> void;
    auto destroy() const -> void;
};

class VertexBuffer : public GlBuffer
{
public:
    explicit VertexBuffer() = default;
    explicit VertexBuffer(std::ranges::contiguous_range auto&& data, BufferUsage usage);

    ZTH_NO_COPY(VertexBuffer)

    VertexBuffer(VertexBuffer&& other) noexcept;
    auto operator=(VertexBuffer&& other) noexcept -> VertexBuffer&;

    ~VertexBuffer() override = default;

    auto buffer_data(std::ranges::contiguous_range auto&& data, BufferUsage usage) -> void;

    auto bind() const -> void override;
    auto unbind() const -> void override;

    auto free() -> void override;

    [[nodiscard]] auto layout() const -> auto& { return _layout; }
    [[nodiscard]] auto stride() const { return _stride; }

private:
    VertexLayout _layout;
    GLsizei _stride;
};

class IndexBuffer : public GlBuffer
{
public:
    explicit IndexBuffer() = default;
    explicit IndexBuffer(std::ranges::contiguous_range auto&& data, BufferUsage usage);

    ZTH_NO_COPY(IndexBuffer)

    IndexBuffer(IndexBuffer&& other) noexcept;
    auto operator=(IndexBuffer&& other) noexcept -> IndexBuffer&;

    ~IndexBuffer() override = default;

    auto buffer_data(std::ranges::contiguous_range auto&& data, BufferUsage usage) -> void;

    auto bind() const -> void override;
    auto unbind() const -> void override;

    auto free() -> void override;

    [[nodiscard]] auto index_type() const -> GLenum { return _index_type; }

private:
    GLenum _index_type;
};

} // namespace zth

#include "GlBuffer.inl"
