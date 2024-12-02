#pragma once

#include "Zenith/Platform/OpenGl/GlUtils.hpp"

namespace zth {

template<typename T> GlBuffer::GlBuffer(std::span<const T> data, BufferUsage usage) : GlBuffer()
{
    buffer_data(data, usage);
}

template<typename T, usize DataSize>
GlBuffer::GlBuffer(std::span<const T, DataSize> data, BufferUsage usage)
    : GlBuffer(std::span<const T, std::dynamic_extent>{ data }, usage)
{}

template<typename T, usize DataSize>
GlBuffer::GlBuffer(const std::array<T, DataSize>& data, BufferUsage usage) : GlBuffer(std::span{ data }, usage)
{}

template<typename T>
GlBuffer::GlBuffer(const std::vector<T>& data, BufferUsage usage) : GlBuffer(std::span{ data }, usage)
{}

template<typename T> auto GlBuffer::buffer_data(std::span<const T> data, BufferUsage usage) -> void
{
    _size = static_cast<GLsizei>(data.size_bytes());
    glNamedBufferData(_id, _size, data.data(), to_gl_enum(usage));
}

template<typename T, usize DataSize>
auto GlBuffer::buffer_data(std::span<const T, DataSize> data, BufferUsage usage) -> void
{
    buffer_data(std::span<const T, std::dynamic_extent>{ data }, usage);
}

template<typename T, usize DataSize>
auto GlBuffer::buffer_data(const std::array<T, DataSize>& data, BufferUsage usage) -> void
{
    buffer_data(std::span{ data }, usage);
}

template<typename T> auto GlBuffer::buffer_data(const std::vector<T>& data, BufferUsage usage) -> void
{
    buffer_data(std::span{ data }, usage);
}

template<typename T>
VertexBuffer::VertexBuffer(std::span<const T> data, BufferUsage usage)
    : GlBuffer(data, usage), _layout(VertexLayout::from_vertex<T>()), _stride(sizeof(T))
{}

template<typename T, usize DataSize>
VertexBuffer::VertexBuffer(std::span<const T, DataSize> data, BufferUsage usage)
    : VertexBuffer(std::span<const T, std::dynamic_extent>{ data }, usage)
{}

template<typename T, usize DataSize>
VertexBuffer::VertexBuffer(const std::array<T, DataSize>& data, BufferUsage usage)
    : VertexBuffer(std::span{ data }, usage)
{}

template<typename T>
VertexBuffer::VertexBuffer(const std::vector<T>& data, BufferUsage usage) : VertexBuffer(std::span{ data }, usage)
{}

template<typename T> auto VertexBuffer::buffer_data(std::span<const T> data, BufferUsage usage) -> void
{
    _layout = VertexLayout::from_vertex<T>();
    _stride = sizeof(T);
    GlBuffer::buffer_data(data, usage);
}

template<typename T, usize DataSize>
auto VertexBuffer::buffer_data(std::span<const T, DataSize> data, BufferUsage usage) -> void
{
    buffer_data(std::span<const T, std::dynamic_extent>{ data }, usage);
}

template<typename T, usize DataSize>
auto VertexBuffer::buffer_data(const std::array<T, DataSize>& data, BufferUsage usage) -> void
{
    buffer_data(std::span{ data }, usage);
}

template<typename T> auto VertexBuffer::buffer_data(const std::vector<T>& data, BufferUsage usage) -> void
{
    buffer_data(std::span{ data }, usage);
}

template<typename T>
IndexBuffer::IndexBuffer(std::span<const T> data, BufferUsage usage)
    : GlBuffer(data, usage), _index_type(to_gl_enum<T>())
{}

template<typename T, usize DataSize>
IndexBuffer::IndexBuffer(std::span<const T, DataSize> data, BufferUsage usage)
    : IndexBuffer(std::span<const T, std::dynamic_extent>{ data }, usage)
{}

template<typename T, usize DataSize>
IndexBuffer::IndexBuffer(const std::array<T, DataSize>& data, BufferUsage usage) : IndexBuffer(std::span{ data }, usage)
{}

template<typename T>
IndexBuffer::IndexBuffer(const std::vector<T>& data, BufferUsage usage) : IndexBuffer(std::span{ data }, usage)
{}

template<typename T> auto IndexBuffer::buffer_data(std::span<const T> data, BufferUsage usage) -> void
{
    _index_type = to_gl_enum<T>();
    GlBuffer::buffer_data(data, usage);
}

template<typename T, usize DataSize>
auto IndexBuffer::buffer_data(std::span<const T, DataSize> data, BufferUsage usage) -> void
{
    buffer_data(std::span<const T, std::dynamic_extent>{ data }, usage);
}

template<typename T, usize DataSize>
auto IndexBuffer::buffer_data(const std::array<T, DataSize>& data, BufferUsage usage) -> void
{
    buffer_data(std::span<const T>{ data }, usage);
}

template<typename T> auto IndexBuffer::buffer_data(const std::vector<T>& data, BufferUsage usage) -> void
{
    buffer_data(std::span<const T>{ data }, usage);
}

} // namespace zth
