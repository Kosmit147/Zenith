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

    ~GlBuffer();

    auto buffer_data(std::ranges::contiguous_range auto&& data) -> void;

    auto buffer_sub_data(std::ranges::contiguous_range auto&& data, GLintptr offset = 0) const -> void;
    auto buffer_sub_data(auto&& object, GLintptr offset = 0) const -> void;
    auto buffer_sub_data(const void* data, GLintptr offset, GLsizeiptr size_bytes) const -> void;

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
    explicit VertexBuffer(GLsizei size);
    explicit VertexBuffer(std::ranges::contiguous_range auto&& data);

    ZTH_NO_COPY(VertexBuffer)

    VertexBuffer(VertexBuffer&& other) noexcept;
    auto operator=(VertexBuffer&& other) noexcept -> VertexBuffer&;

    ~VertexBuffer() = default;

    auto buffer_data(std::ranges::contiguous_range auto&& data) -> void;

    auto bind() const -> void;
    static auto unbind() -> void;

    auto set_layout(const VertexLayout& layout) -> void;
    auto set_layout(VertexLayout&& layout) -> void;
    auto set_stride(GLsizei stride) -> void;

    [[nodiscard]] auto layout() const -> auto& { return _layout; }
    [[nodiscard]] auto stride() const { return _stride; }

private:
    VertexLayout _layout = {};
    GLsizei _stride = 0;
};

class IndexBuffer : public GlBuffer
{
public:
    explicit IndexBuffer() = default;
    explicit IndexBuffer(GLsizei size);
    explicit IndexBuffer(std::ranges::contiguous_range auto&& data);

    ZTH_NO_COPY(IndexBuffer)

    IndexBuffer(IndexBuffer&& other) noexcept;
    auto operator=(IndexBuffer&& other) noexcept -> IndexBuffer&;

    ~IndexBuffer() = default;

    auto buffer_data(std::ranges::contiguous_range auto&& data) -> void;

    auto bind() const -> void;
    static auto unbind() -> void;

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
    ~UniformBuffer() = default;

    auto bind() const -> void;
    static auto unbind() -> void;

    auto set_binding_index(GLuint index) const -> void;
};

class InstanceBuffer : public VertexBuffer
{
public:
    using VertexBuffer::VertexBuffer;
    ZTH_NO_COPY(InstanceBuffer)
    ZTH_DEFAULT_MOVE(InstanceBuffer)
    ~InstanceBuffer() = default;
};

} // namespace zth

#include "GlBuffer.inl"
