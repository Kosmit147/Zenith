#pragma once

#include <glad/glad.h>

#include <array>
#include <span>
#include <vector>

#include "Zenith/Core/Typedefs.hpp"
#include "Zenith/Platform/OpenGl/BufferUsage.hpp"
#include "Zenith/Utility/Utility.hpp"

namespace zth {

class IndexBuffer
{
public:
    explicit IndexBuffer() { create(); }

    template<typename T> explicit IndexBuffer(std::span<const T> data, BufferUsage usage) : IndexBuffer()
    {
        buffer_data(data, usage);
    }

    template<typename T, usize DataSize>
    explicit IndexBuffer(std::span<const T, DataSize> data, BufferUsage usage) : IndexBuffer()
    {
        buffer_data(data, usage);
    }

    template<typename T, usize DataSize>
    explicit IndexBuffer(const std::array<T, DataSize>& data, BufferUsage usage) : IndexBuffer()
    {
        buffer_data(data, usage);
    }

    template<typename T> explicit IndexBuffer(const std::vector<T>& data, BufferUsage usage) : IndexBuffer()
    {
        buffer_data(data, usage);
    }

    ZTH_NO_COPY_NO_MOVE(IndexBuffer)

    ~IndexBuffer() { destroy(); }

    template<typename T> auto buffer_data(std::span<const T> data, BufferUsage usage) -> void
    {
        _size = data.size_bytes();
        glNamedBufferData(_id, _size, data.data(), to_gl_enum(usage));
    }

    template<typename T, usize DataSize> auto buffer_data(std::span<const T, DataSize> data, BufferUsage usage) -> void
    {
        _size = data.size_bytes();
        glNamedBufferData(_id, _size, data.data(), to_gl_enum(usage));
    }

    template<typename T, usize DataSize>
    auto buffer_data(const std::array<T, DataSize>& data, BufferUsage usage) -> void
    {
        _size = DataSize * sizeof(T);
        glNamedBufferData(_id, _size, data.data(), to_gl_enum(usage));
    }

    template<typename T> auto buffer_data(const std::vector<T>& data, BufferUsage usage) -> void
    {
        _size = data.size() * sizeof(T);
        glNamedBufferData(_id, _size, data.data(), to_gl_enum(usage));
    }

    auto bind() const -> void { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id); }
    auto unbind() const -> void { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

    auto free() -> void
    {
        destroy();
        _id = 0;
        _size = 0;
    }

    [[nodiscard]] auto native_handle() const { return _id; }
    [[nodiscard]] auto size() const { return _size; }

private:
    GLuint _id = 0; // 0 is an invalid buffer id
    GLsizei _size = 0;

private:
    auto create() -> void { glCreateBuffers(1, &_id); }
    auto destroy() const -> void { glDeleteBuffers(1, &_id); }
};

} // namespace zth
