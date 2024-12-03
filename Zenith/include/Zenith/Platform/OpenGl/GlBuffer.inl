#pragma once

#include "Zenith/Platform/OpenGl/GlUtils.hpp"

namespace zth {

GlBuffer::GlBuffer(std::ranges::contiguous_range auto&& data, BufferUsage usage) : GlBuffer()
{
    buffer_data(data, usage);
}

auto GlBuffer::buffer_data(std::ranges::contiguous_range auto&& data, BufferUsage usage) -> void
{
    using T = std::ranges::range_value_t<decltype(data)>;
    _size = static_cast<GLsizei>(data.size());
    _size_bytes = _size * sizeof(T);
    glNamedBufferData(_id, _size_bytes, data.data(), to_gl_enum(usage));
}

VertexBuffer::VertexBuffer(std::ranges::contiguous_range auto&& data, BufferUsage usage)
    : GlBuffer(data, usage), _layout(VertexLayout::from_vertex<std::ranges::range_value_t<decltype(data)>>()),
      _stride(sizeof(std::ranges::range_value_t<decltype(data)>))
{}

auto VertexBuffer::buffer_data(std::ranges::contiguous_range auto&& data, BufferUsage usage) -> void
{
    using T = std::ranges::range_value_t<decltype(data)>;
    _layout = VertexLayout::from_vertex<T>();
    _stride = sizeof(T);
    GlBuffer::buffer_data(data, usage);
}

IndexBuffer::IndexBuffer(std::ranges::contiguous_range auto&& data, BufferUsage usage)
    : GlBuffer(data, usage), _index_type(to_gl_enum<std::ranges::range_value_t<decltype(data)>>())
{}

auto IndexBuffer::buffer_data(std::ranges::contiguous_range auto&& data, BufferUsage usage) -> void
{
    using T = std::ranges::range_value_t<decltype(data)>;
    _index_type = to_gl_enum<T>();
    GlBuffer::buffer_data(data, usage);
}

} // namespace zth
