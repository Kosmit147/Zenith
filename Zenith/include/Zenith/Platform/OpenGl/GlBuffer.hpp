#pragma once

#include <glad/glad.h>

#include <array>
#include <span>
#include <vector>

#include "Zenith/Platform/OpenGl/BufferUsage.hpp"
#include "Zenith/Utility/Utility.hpp"

namespace zth {

class GlBuffer
{
public:
    explicit GlBuffer() { create(); }

    template<typename T> explicit GlBuffer(std::span<const T> data, BufferUsage usage) : GlBuffer()
    {
        buffer_data(data, usage);
    }

    template<typename T, usize DataSize>
    explicit GlBuffer(std::span<const T, DataSize> data, BufferUsage usage) : GlBuffer()
    {
        buffer_data(data, usage);
    }

    template<typename T, usize DataSize>
    explicit GlBuffer(const std::array<T, DataSize>& data, BufferUsage usage) : GlBuffer()
    {
        buffer_data(data, usage);
    }

    template<typename T> explicit GlBuffer(const std::vector<T>& data, BufferUsage usage) : GlBuffer()
    {
        buffer_data(data, usage);
    }

    ZTH_NO_COPY_NO_MOVE(GlBuffer)

    virtual ~GlBuffer() { destroy(); }

    template<typename T> auto buffer_data(std::span<const T> data, BufferUsage usage) -> void
    {
        glNamedBufferData(_id, data.size_bytes(), data.data(), to_gl_enum(usage));
    }

    template<typename T, usize DataSize> auto buffer_data(std::span<const T, DataSize> data, BufferUsage usage) -> void
    {
        glNamedBufferData(_id, data.size_bytes(), data.data(), to_gl_enum(usage));
    }

    template<typename T, usize DataSize>
    auto buffer_data(const std::array<T, DataSize>& data, BufferUsage usage) -> void
    {
        glNamedBufferData(_id, DataSize * sizeof(T), data.data(), to_gl_enum(usage));
    }

    template<typename T> auto buffer_data(const std::vector<T>& data, BufferUsage usage) -> void
    {
        glNamedBufferData(_id, data.size() * sizeof(T), data.data(), to_gl_enum(usage));
    }

    virtual auto bind() const -> void = 0;
    virtual auto unbind() const -> void = 0;

    auto free() -> void
    {
        destroy();
        _id = 0;
    }

    [[nodiscard]] auto native_handle() const { return _id; }

private:
    GLuint _id = 0; // 0 is an invalid buffer id

private:
    auto create() -> void { glCreateBuffers(1, &_id); }
    auto destroy() const -> void { glDeleteBuffers(1, &_id); }
};

template<typename VertexType> class VertexBuffer : public GlBuffer
{
public:
    explicit VertexBuffer() = default;

    explicit VertexBuffer(std::span<const VertexType> vertices, BufferUsage usage) : GlBuffer(vertices, usage) {}

    template<usize DataSize>
    explicit VertexBuffer(std::span<const VertexType, DataSize> vertices, BufferUsage usage) : GlBuffer(vertices, usage)
    {}

    template<usize DataSize>
    explicit VertexBuffer(const std::array<VertexType, DataSize>& vertices, BufferUsage usage)
        : GlBuffer(vertices, usage)
    {}

    explicit VertexBuffer(const std::vector<VertexType>& vertices, BufferUsage usage) : GlBuffer(vertices, usage) {}

    ZTH_NO_COPY_NO_MOVE(VertexBuffer)

    ~VertexBuffer() override = default;

    auto buffer_data(std::span<const VertexType> vertices, BufferUsage usage) -> void
    {
        GlBuffer::buffer_data(vertices, usage);
    }

    template<usize DataSize> auto buffer_data(std::span<const VertexType, DataSize> vertices, BufferUsage usage) -> void
    {
        GlBuffer::buffer_data(vertices, usage);
    }

    template<usize DataSize>
    auto buffer_data(const std::array<VertexType, DataSize>& vertices, BufferUsage usage) -> void
    {
        GlBuffer::buffer_data(vertices, usage);
    }

    auto buffer_data(const std::vector<VertexType>& vertices, BufferUsage usage) -> void
    {
        GlBuffer::buffer_data(vertices, usage);
    }

    auto bind() const -> void override { glBindBuffer(GL_ARRAY_BUFFER, native_handle()); }
    auto unbind() const -> void override { glBindBuffer(GL_ARRAY_BUFFER, 0); }
};

template<typename IndexType> class IndexBuffer : public GlBuffer
{
public:
    explicit IndexBuffer() = default;

    explicit IndexBuffer(std::span<const IndexType> indices, BufferUsage usage) : GlBuffer(indices, usage) {}

    template<usize DataSize>
    explicit IndexBuffer(std::span<const IndexType, DataSize> indices, BufferUsage usage) : GlBuffer(indices, usage)
    {}

    template<usize DataSize>
    explicit IndexBuffer(const std::array<IndexType, DataSize>& indices, BufferUsage usage) : GlBuffer(indices, usage)
    {}

    explicit IndexBuffer(const std::vector<IndexType>& indices, BufferUsage usage) : GlBuffer(indices, usage) {}

    ZTH_NO_COPY_NO_MOVE(IndexBuffer)

    ~IndexBuffer() override = default;

    auto buffer_data(std::span<const IndexType> indices, BufferUsage usage) -> void
    {
        GlBuffer::buffer_data(indices, usage);
    }

    template<usize DataSize> auto buffer_data(std::span<const IndexType, DataSize> indices, BufferUsage usage) -> void
    {
        GlBuffer::buffer_data(indices, usage);
    }

    template<usize DataSize> auto buffer_data(const std::array<IndexType, DataSize>& indices, BufferUsage usage) -> void
    {
        GlBuffer::buffer_data(indices, usage);
    }

    auto buffer_data(const std::vector<IndexType>& indices, BufferUsage usage) -> void
    {
        GlBuffer::buffer_data(indices, usage);
    }

    auto bind() const -> void override { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, native_handle()); }
    auto unbind() const -> void override { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
};

} // namespace zth