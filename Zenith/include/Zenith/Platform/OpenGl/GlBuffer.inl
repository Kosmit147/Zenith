#pragma once

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
    glNamedBufferData(_id, data.size_bytes(), data.data(), to_gl_enum(usage));
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

template<typename VertexType>
VertexBuffer<VertexType>::VertexBuffer(std::span<const VertexType> vertices, BufferUsage usage)
    : GlBuffer(vertices, usage)
{}

template<typename VertexType>
template<usize DataSize>
VertexBuffer<VertexType>::VertexBuffer(std::span<const VertexType, DataSize> vertices, BufferUsage usage)
    : GlBuffer(vertices, usage)
{}

template<typename VertexType>
template<usize DataSize>
VertexBuffer<VertexType>::VertexBuffer(const std::array<VertexType, DataSize>& vertices, BufferUsage usage)
    : GlBuffer(vertices, usage)
{}

template<typename VertexType>
VertexBuffer<VertexType>::VertexBuffer(const std::vector<VertexType>& vertices, BufferUsage usage)
    : GlBuffer(vertices, usage)
{}

template<typename VertexType>
auto VertexBuffer<VertexType>::buffer_data(std::span<const VertexType> vertices, BufferUsage usage) -> void
{
    GlBuffer::buffer_data(vertices, usage);
}

template<typename VertexType>
template<usize DataSize>
auto VertexBuffer<VertexType>::buffer_data(std::span<const VertexType, DataSize> vertices, BufferUsage usage) -> void
{
    GlBuffer::buffer_data(vertices, usage);
}

template<typename VertexType>
template<usize DataSize>
auto VertexBuffer<VertexType>::buffer_data(const std::array<VertexType, DataSize>& vertices, BufferUsage usage) -> void
{
    GlBuffer::buffer_data(vertices, usage);
}

template<typename VertexType>
auto VertexBuffer<VertexType>::buffer_data(const std::vector<VertexType>& vertices, BufferUsage usage) -> void
{
    GlBuffer::buffer_data(vertices, usage);
}

template<typename IndexType>
IndexBuffer<IndexType>::IndexBuffer(std::span<const IndexType> indices, BufferUsage usage) : GlBuffer(indices, usage)
{}

template<typename IndexType>
template<usize DataSize>
IndexBuffer<IndexType>::IndexBuffer(std::span<const IndexType, DataSize> indices, BufferUsage usage)
    : GlBuffer(indices, usage)
{}

template<typename IndexType>
template<usize DataSize>
IndexBuffer<IndexType>::IndexBuffer(const std::array<IndexType, DataSize>& indices, BufferUsage usage)
    : GlBuffer(indices, usage)
{}

template<typename IndexType>
IndexBuffer<IndexType>::IndexBuffer(const std::vector<IndexType>& indices, BufferUsage usage) : GlBuffer(indices, usage)
{}

template<typename IndexType>
auto IndexBuffer<IndexType>::buffer_data(std::span<const IndexType> indices, BufferUsage usage) -> void
{
    GlBuffer::buffer_data(indices, usage);
}

template<typename IndexType>
template<usize DataSize>
auto IndexBuffer<IndexType>::buffer_data(std::span<const IndexType, DataSize> indices, BufferUsage usage) -> void
{
    GlBuffer::buffer_data(indices, usage);
}

template<typename IndexType>
template<usize DataSize>
auto IndexBuffer<IndexType>::buffer_data(const std::array<IndexType, DataSize>& indices, BufferUsage usage) -> void
{
    GlBuffer::buffer_data(indices, usage);
}

template<typename IndexType>
auto IndexBuffer<IndexType>::buffer_data(const std::vector<IndexType>& indices, BufferUsage usage) -> void
{
    GlBuffer::buffer_data(indices, usage);
}

} // namespace zth
