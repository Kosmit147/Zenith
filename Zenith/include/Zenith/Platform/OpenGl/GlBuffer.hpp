#pragma once

#include <glad/glad.h>

#include <ranges>

#include "Zenith/Platform/OpenGl/VertexLayout.hpp"
#include "Zenith/Utility/Utility.hpp"

namespace zth {

class GlBuffer
{
public:
    explicit GlBuffer();
    explicit GlBuffer(GLsizei size);
    explicit GlBuffer(std::ranges::contiguous_range auto&& data);

    ZTH_NO_COPY(GlBuffer)

    GlBuffer(GlBuffer&& other) noexcept;
    auto operator=(GlBuffer&& other) noexcept -> GlBuffer&;

    virtual ~GlBuffer();

    auto buffer_data(std::ranges::contiguous_range auto&& data) -> void;

    virtual auto bind() const -> void = 0;
    virtual auto unbind() const -> void = 0;

    virtual auto free() -> void;

    [[nodiscard]] auto native_handle() const { return _id; }
    [[nodiscard]] auto size_bytes() const { return _size_bytes; }

private:
    GLuint _id = GL_NONE;
    GLsizei _size_bytes = 0;

private:
    auto create() -> void;
    auto destroy() const -> void;
};

class VertexBuffer : public GlBuffer
{
public:
    explicit VertexBuffer() = default;
    explicit VertexBuffer(std::ranges::contiguous_range auto&& data);

    ZTH_NO_COPY(VertexBuffer)

    VertexBuffer(VertexBuffer&& other) noexcept;
    auto operator=(VertexBuffer&& other) noexcept -> VertexBuffer&;

    ~VertexBuffer() override = default;

    auto buffer_data(std::ranges::contiguous_range auto&& data) -> void;

    auto bind() const -> void override;
    auto unbind() const -> void override;

    auto free() -> void override;

    [[nodiscard]] auto layout() const -> auto& { return _layout; }
    [[nodiscard]] auto size() const { return _size; }
    [[nodiscard]] auto stride() const { return _stride; }

private:
    VertexLayout _layout = {};
    GLsizei _size = 0;
    GLsizei _stride = 0;
};

class IndexBuffer : public GlBuffer
{
public:
    explicit IndexBuffer() = default;
    explicit IndexBuffer(std::ranges::contiguous_range auto&& data);

    ZTH_NO_COPY(IndexBuffer)

    IndexBuffer(IndexBuffer&& other) noexcept;
    auto operator=(IndexBuffer&& other) noexcept -> IndexBuffer&;

    ~IndexBuffer() override = default;

    auto buffer_data(std::ranges::contiguous_range auto&& data) -> void;

    auto bind() const -> void override;
    auto unbind() const -> void override;

    auto free() -> void override;

    [[nodiscard]] auto index_type() const { return _index_type; }
    [[nodiscard]] auto size() const { return _size; }

private:
    GLenum _index_type = GL_NONE;
    GLsizei _size = 0;
};

class UniformBuffer : public GlBuffer
{
public:
    using GlBuffer::GlBuffer;
    ZTH_NO_COPY(UniformBuffer)
    ZTH_DEFAULT_MOVE(UniformBuffer)
    ~UniformBuffer() override = default;

    auto buffer_sub_data(std::ranges::contiguous_range auto&& data, GLintptr offset = 0) const -> void;
    auto buffer_sub_data(const void* data, GLintptr offset, GLsizeiptr size_bytes) const -> void;

    auto bind() const -> void override;
    auto unbind() const -> void override;

    auto set_binding_index(GLuint index) const -> void;
};

} // namespace zth

#include "GlBuffer.inl"
