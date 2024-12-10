#pragma once

#include "Zenith/Platform/OpenGl/GlUtils.hpp"

namespace zth {

GlBuffer::GlBuffer(std::ranges::contiguous_range auto&& data) : GlBuffer()
{
    buffer_data(data);
}

auto GlBuffer::buffer_data(std::ranges::contiguous_range auto&& data) -> void
{
    using T = std::ranges::range_value_t<decltype(data)>;
    _size_bytes = static_cast<GLsizei>(data.size() * sizeof(T));
    glNamedBufferStorage(_id, _size_bytes, data.data(), GL_DYNAMIC_STORAGE_BIT);
}

VertexBuffer::VertexBuffer(std::ranges::contiguous_range auto&& data)
    : GlBuffer(data), _layout(VertexLayout::from_vertex<std::ranges::range_value_t<decltype(data)>>()),
      _size(static_cast<GLsizei>(data.size())), _stride(sizeof(std::ranges::range_value_t<decltype(data)>))
{}

auto VertexBuffer::buffer_data(std::ranges::contiguous_range auto&& data) -> void
{
    using T = std::ranges::range_value_t<decltype(data)>;
    _layout = VertexLayout::from_vertex<T>();
    _size = static_cast<GLsizei>(data.size());
    _stride = sizeof(T);
    GlBuffer::buffer_data(data);
}

IndexBuffer::IndexBuffer(std::ranges::contiguous_range auto&& data)
    : GlBuffer(data), _index_type(to_gl_enum<std::ranges::range_value_t<decltype(data)>>()),
      _size(static_cast<GLsizei>(data.size()))
{}

auto IndexBuffer::buffer_data(std::ranges::contiguous_range auto&& data) -> void
{
    using T = std::ranges::range_value_t<decltype(data)>;
    _index_type = to_gl_enum<T>();
    _size = static_cast<GLsizei>(data.size());
    GlBuffer::buffer_data(data);
}

auto UniformBuffer::buffer_sub_data(std::ranges::contiguous_range auto&& data, GLintptr offset) const -> void
{
    using T = std::ranges::range_value_t<decltype(data)>;
    auto data_size_bytes = static_cast<GLsizeiptr>(sizeof(T) * data.size());

    ZTH_ASSERT(data_size_bytes + offset <= size_bytes());

    glNamedBufferSubData(native_handle(), offset, data_size_bytes, data.data());
}

} // namespace zth
