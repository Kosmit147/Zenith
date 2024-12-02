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

    template<typename T> explicit GlBuffer(std::span<const T> data, BufferUsage usage);
    template<typename T, usize DataSize> explicit GlBuffer(std::span<const T, DataSize> data, BufferUsage usage);
    template<typename T, usize DataSize> explicit GlBuffer(const std::array<T, DataSize>& data, BufferUsage usage);
    template<typename T> explicit GlBuffer(const std::vector<T>& data, BufferUsage usage);

    ZTH_NO_COPY_NO_MOVE(GlBuffer)
    virtual ~GlBuffer() { destroy(); }

    template<typename T> auto buffer_data(std::span<const T> data, BufferUsage usage) -> void;
    template<typename T, usize DataSize> auto buffer_data(std::span<const T, DataSize> data, BufferUsage usage) -> void;
    template<typename T, usize DataSize>
    auto buffer_data(const std::array<T, DataSize>& data, BufferUsage usage) -> void;
    template<typename T> auto buffer_data(const std::vector<T>& data, BufferUsage usage) -> void;

    virtual auto bind() const -> void = 0;
    virtual auto unbind() const -> void = 0;

    auto free() -> void
    {
        destroy();
        _id = 0;
        _size = 0;
    }

    [[nodiscard]] auto native_handle() const { return _id; }
    [[nodiscard]] auto size() const { return _size; }

private:
    GLuint _id = GL_NONE;
    GLsizei _size = 0;

private:
    auto create() -> void { glCreateBuffers(1, &_id); }
    auto destroy() const -> void { glDeleteBuffers(1, &_id); }
};

class VertexBuffer : public GlBuffer
{
public:
    template<typename T> explicit VertexBuffer(std::span<const T> data, BufferUsage usage);
    template<typename T, usize DataSize> explicit VertexBuffer(std::span<const T, DataSize> data, BufferUsage usage);
    template<typename T, usize DataSize> explicit VertexBuffer(const std::array<T, DataSize>& data, BufferUsage usage);
    template<typename T> explicit VertexBuffer(const std::vector<T>& data, BufferUsage usage);

    ZTH_NO_COPY_NO_MOVE(VertexBuffer)
    ~VertexBuffer() override = default;

    template<typename T> auto buffer_data(std::span<const T> data, BufferUsage usage) -> void;
    template<typename T, usize DataSize> auto buffer_data(std::span<const T, DataSize> data, BufferUsage usage) -> void;
    template<typename T, usize DataSize>
    auto buffer_data(const std::array<T, DataSize>& data, BufferUsage usage) -> void;
    template<typename T> auto buffer_data(const std::vector<T>& data, BufferUsage usage) -> void;

    auto bind() const -> void override { glBindBuffer(GL_ARRAY_BUFFER, native_handle()); }
    auto unbind() const -> void override { glBindBuffer(GL_ARRAY_BUFFER, 0); }

    [[nodiscard]] auto stride() const { return _stride; }

private:
    GLsizei _stride;
};

class IndexBuffer : public GlBuffer
{
public:
    template<typename T> explicit IndexBuffer(std::span<const T> data, BufferUsage usage);
    template<typename T, usize DataSize> explicit IndexBuffer(std::span<const T, DataSize> data, BufferUsage usage);
    template<typename T, usize DataSize> explicit IndexBuffer(const std::array<T, DataSize>& data, BufferUsage usage);
    template<typename T> explicit IndexBuffer(const std::vector<T>& data, BufferUsage usage);

    ZTH_NO_COPY_NO_MOVE(IndexBuffer)
    ~IndexBuffer() override = default;

    template<typename T> auto buffer_data(std::span<const T> data, BufferUsage usage) -> void;
    template<typename T, usize DataSize> auto buffer_data(std::span<const T, DataSize> data, BufferUsage usage) -> void;
    template<typename T, usize DataSize>
    auto buffer_data(const std::array<T, DataSize>& data, BufferUsage usage) -> void;
    template<typename T> auto buffer_data(const std::vector<T>& data, BufferUsage usage) -> void;

    auto bind() const -> void override { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, native_handle()); }
    auto unbind() const -> void override { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

    [[nodiscard]] auto type() const -> GLenum { return _type; }

private:
    GLenum _type;
};

} // namespace zth

#include "GlBuffer.inl"
