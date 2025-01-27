#pragma once

#include "zenith/core/assert.hpp"
#include "zenith/gl/util.hpp"

namespace zth::gl {

// --------------------------- GlBuffer ---------------------------

auto Buffer::create_static(std::ranges::contiguous_range auto&& data) -> Buffer
{
    Buffer buffer;
    buffer.init_static(data);
    return buffer;
}

auto Buffer::create_dynamic(std::ranges::contiguous_range auto&& data, BufferUsage usage) -> Buffer
{
    Buffer buffer;
    buffer.init_dynamic(data, usage);
    return buffer;
}

auto Buffer::init_static(std::ranges::contiguous_range auto&& data) -> void
{
    ZTH_ASSERT(_state == BufferState::Uninitialized);

    using T = std::ranges::range_value_t<decltype(data)>;
    _size_bytes = static_cast<GLsizei>(std::size(data) * sizeof(T));
    glNamedBufferStorage(_id, _size_bytes, std::data(data), GL_DYNAMIC_STORAGE_BIT);

    _state = BufferState::InitializedStatic;
}

auto Buffer::init_dynamic(std::ranges::contiguous_range auto&& data, BufferUsage usage) -> void
{
    ZTH_ASSERT(_state != BufferState::InitializedStatic);

    using T = std::ranges::range_value_t<decltype(data)>;
    _size_bytes = static_cast<GLsizei>(std::size(data) * sizeof(T));
    _usage = usage;
    glNamedBufferData(_id, _size_bytes, std::data(data), to_gl_enum(*_usage));

    _state = BufferState::InitializedDynamic;
}

auto Buffer::buffer_data(std::ranges::contiguous_range auto&& data, usize offset) -> void
{
    using T = std::ranges::range_value_t<decltype(data)>;
    auto data_size_bytes = static_cast<GLsizeiptr>(std::size(data) * sizeof(T));
    buffer_data(std::data(data), offset, data_size_bytes);
}

auto Buffer::buffer_data(auto&& object, usize offset) -> void
{
    using T = decltype(object);
    auto data_size_bytes = static_cast<GLsizeiptr>(sizeof(T));
    buffer_data(&object, offset, data_size_bytes);
}

// --------------------------- VertexBuffer ---------------------------

auto VertexBuffer::create_static(std::ranges::contiguous_range auto&& data) -> VertexBuffer
{
    VertexBuffer buffer;
    buffer.init_static(data);
    return buffer;
}

auto VertexBuffer::create_dynamic(std::ranges::contiguous_range auto&& data, BufferUsage usage) -> VertexBuffer
{
    VertexBuffer buffer;
    buffer.init_dynamic(data, usage);
    return buffer;
}

auto VertexBuffer::init_static(std::ranges::contiguous_range auto&& data) -> void
{
    using T = std::ranges::range_value_t<decltype(data)>;
    _stride = sizeof(T);
    _buffer.init_static(data);
}

auto VertexBuffer::init_dynamic(std::ranges::contiguous_range auto&& data, BufferUsage usage) -> void
{
    using T = std::ranges::range_value_t<decltype(data)>;
    _stride = sizeof(T);
    _buffer.init_dynamic(data, usage);
}

auto VertexBuffer::buffer_data(std::ranges::contiguous_range auto&& data, usize offset) -> void
{
    using T = std::ranges::range_value_t<decltype(data)>;
    _stride = sizeof(T);
    _buffer.buffer_data(data, offset);
}

// --------------------------- IndexBuffer ---------------------------

auto IndexBuffer::create_static(std::ranges::contiguous_range auto&& data) -> IndexBuffer
{
    IndexBuffer buffer;
    buffer.init_static(data);
    return buffer;
}

auto IndexBuffer::create_dynamic(std::ranges::contiguous_range auto&& data, BufferUsage usage) -> IndexBuffer
{
    IndexBuffer buffer;
    buffer.init_dynamic(data, usage);
    return buffer;
}

auto IndexBuffer::init_static(std::ranges::contiguous_range auto&& data) -> void
{
    using T = std::ranges::range_value_t<decltype(data)>;
    _index_type = to_gl_enum<T>();
    _size = static_cast<GLsizei>(std::size(data));
    _buffer.init_static(data);
}

auto IndexBuffer::init_dynamic(std::ranges::contiguous_range auto&& data, BufferUsage usage) -> void
{
    using T = std::ranges::range_value_t<decltype(data)>;
    _index_type = to_gl_enum<T>();
    _size = static_cast<GLsizei>(std::size(data));
    _buffer.init_dynamic(data, usage);
}

auto IndexBuffer::buffer_data(std::ranges::contiguous_range auto&& data, usize offset) -> void
{
    using T = std::ranges::range_value_t<decltype(data)>;
    _index_type = to_gl_enum<T>();
    _size = static_cast<GLsizei>(std::size(data));
    _buffer.buffer_data(data, offset);
}

// --------------------------- InstanceBuffer ---------------------------

auto InstanceBuffer::create_static(std::ranges::contiguous_range auto&& data) -> InstanceBuffer
{
    InstanceBuffer buffer;
    buffer.init_static(data);
    return buffer;
}

auto InstanceBuffer::create_dynamic(std::ranges::contiguous_range auto&& data, BufferUsage usage) -> InstanceBuffer
{
    InstanceBuffer buffer;
    buffer.init_dynamic(data, usage);
    return buffer;
}

// --------------------------- UniformBuffer ---------------------------

auto UniformBuffer::create_static(std::ranges::contiguous_range auto&& data) -> UniformBuffer
{
    UniformBuffer buffer;
    buffer.init_static(data);
    return buffer;
}

auto UniformBuffer::create_static(std::ranges::contiguous_range auto&& data, u32 binding_index) -> UniformBuffer
{
    UniformBuffer buffer;
    buffer.init_static(data, binding_index);
    return buffer;
}

auto UniformBuffer::init_static(std::ranges::contiguous_range auto&& data) -> void
{
    _buffer.init_static(data);
}

auto UniformBuffer::init_static(std::ranges::contiguous_range auto&& data, u32 binding_index) -> void
{
    _buffer.init_static(data);
    set_binding_index(binding_index);
}

auto UniformBuffer::buffer_data(std::ranges::contiguous_range auto&& data, usize offset) -> void
{
    _buffer.buffer_data(data, offset);
}

auto UniformBuffer::buffer_data(auto&& object, usize offset) -> void
{
    _buffer.buffer_data(object, offset);
}

} // namespace zth::gl
