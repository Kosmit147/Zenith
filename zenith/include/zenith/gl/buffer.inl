#pragma once

#include <memory>

#include "zenith/core/assert.hpp"
#include "zenith/stl/span.hpp"

namespace zth::gl {

// --------------------------- Buffer ---------------------------

auto Buffer::create_static_with_data(auto&& data) -> Buffer
{
    Buffer buffer;
    buffer.init_static_with_data(data);
    return buffer;
}

auto Buffer::create_static_with_data(std::ranges::contiguous_range auto&& data) -> Buffer
{
    Buffer buffer;
    buffer.init_static_with_data(data);
    return buffer;
}

auto Buffer::create_dynamic_with_data(auto&& data, BufferUsage usage) -> Buffer
{
    Buffer buffer;
    buffer.init_dynamic_with_data(data, usage);
    return buffer;
}

auto Buffer::create_dynamic_with_data(std::ranges::contiguous_range auto&& data, BufferUsage usage) -> Buffer
{
    Buffer buffer;
    buffer.init_dynamic_with_data(data, usage);
    return buffer;
}

auto Buffer::init_static_with_data(auto&& data) -> void
{
    init_static_with_data(std::as_bytes(std::span{ std::addressof(data), 1 }));
}

auto Buffer::init_static_with_data(std::ranges::contiguous_range auto&& data) -> void
{
    init_static_with_data(make_dynamic_span(std::as_bytes(std::span{ data })));
}

auto Buffer::init_dynamic_with_data(auto&& data, BufferUsage usage) -> void
{
    init_dynamic_with_data(std::as_bytes(std::span{ std::addressof(data), 1 }), usage);
}

auto Buffer::init_dynamic_with_data(std::ranges::contiguous_range auto&& data, BufferUsage usage) -> void
{
    init_dynamic_with_data(make_dynamic_span(std::as_bytes(std::span{ data })), usage);
}

auto Buffer::buffer_data(auto&& data, u32 offset) -> u32
{
    return buffer_data(std::as_bytes(std::span{ std::addressof(data), 1 }), offset);
}

auto Buffer::buffer_data(std::ranges::contiguous_range auto&& data, u32 offset) -> u32
{
    return buffer_data(make_dynamic_span(std::as_bytes(std::span{ data })), offset);
}

// --------------------------- VertexBuffer ---------------------------

template<VertexRange V>
auto VertexBuffer::create_static_with_data(V&& vertices, const VertexLayout& layout) -> VertexBuffer
{
    VertexBuffer buffer;
    buffer.init_static_with_data(vertices, layout);
    return buffer;
}

template<VertexRange V>
auto VertexBuffer::create_dynamic_with_data(V&& vertices, BufferUsage usage, const VertexLayout& layout) -> VertexBuffer
{
    VertexBuffer buffer;
    buffer.init_dynamic_with_data(vertices, usage, layout);
    return buffer;
}

template<VertexRange V> auto VertexBuffer::init_static_with_data(V&& vertices, const VertexLayout& layout) -> void
{
    _buffer.init_static_with_data(vertices);
    _layout = layout;
}

template<VertexRange V>
auto VertexBuffer::init_dynamic_with_data(V&& vertices, BufferUsage usage, const VertexLayout& layout) -> void
{
    _buffer.init_dynamic_with_data(vertices, usage);
    _layout = layout;
}

auto VertexBuffer::buffer_data(VertexRange auto&& vertices, u32 offset) -> u32
{
    return _buffer.buffer_data(vertices, offset);
}

template<VertexRange V> constexpr auto VertexBuffer::derive_vertex_layout() -> VertexLayout
{
    return VertexLayout::derive_from_vertex<std::ranges::range_value_t<V>>();
}

// --------------------------- IndexBuffer ---------------------------

template<IndexRange I> auto IndexBuffer::create_static_with_data(I&& indices, DataType type) -> IndexBuffer
{
    IndexBuffer buffer;
    buffer.init_static_with_data(indices, type);
    return buffer;
}

template<IndexRange I>
auto IndexBuffer::create_dynamic_with_data(I&& indices, BufferUsage usage, DataType type) -> IndexBuffer
{
    IndexBuffer buffer;
    buffer.init_dynamic_with_data(indices, usage, type);
    return buffer;
}

template<IndexRange I> auto IndexBuffer::init_static_with_data(I&& indices, DataType type) -> void
{
    _buffer.init_static_with_data(indices);
    set_indexing_data_type(type);
}

template<IndexRange I> auto IndexBuffer::init_dynamic_with_data(I&& indices, BufferUsage usage, DataType type) -> void
{
    _buffer.init_dynamic_with_data(indices, usage);
    set_indexing_data_type(type);
}

auto IndexBuffer::buffer_data(IndexRange auto&& indices, u32 offset) -> u32
{
    return _buffer.buffer_data(indices, offset);
}

template<IndexingType T> auto IndexBuffer::set_indexing_data_type() -> void
{
    set_indexing_data_type(to_data_type<T>);
}

template<IndexRange I> constexpr auto IndexBuffer::derive_indexing_data_type() -> DataType
{
    return to_data_type<std::ranges::range_value_t<I>>;
}

// --------------------------- InstanceBuffer ---------------------------

template<VertexRange V>
auto InstanceBuffer::create_static_with_data(V&& data, const VertexLayout& layout) -> InstanceBuffer
{
    InstanceBuffer buffer;
    buffer.init_static_with_data(data, layout);
    return buffer;
}

template<VertexRange V>
auto InstanceBuffer::create_dynamic_with_data(V&& data, BufferUsage usage, const VertexLayout& layout) -> InstanceBuffer
{
    InstanceBuffer buffer;
    buffer.init_dynamic_with_data(data, usage, layout);
    return buffer;
}

// --------------------------- UniformBuffer ---------------------------

auto UniformBuffer::create_static_with_data(auto&& data) -> UniformBuffer
{
    UniformBuffer buffer;
    buffer.init_static_with_data(data);
    return buffer;
}

auto UniformBuffer::create_static_with_data(auto&& data, u32 binding_point) -> UniformBuffer
{
    UniformBuffer buffer;
    buffer.init_static_with_data(data, binding_point);
    return buffer;
}

auto UniformBuffer::create_static_with_data(std::ranges::contiguous_range auto&& data) -> UniformBuffer
{
    UniformBuffer buffer;
    buffer.init_static_with_data(data);
    return buffer;
}

auto UniformBuffer::create_static_with_data(std::ranges::contiguous_range auto&& data, u32 binding_point)
    -> UniformBuffer
{
    UniformBuffer buffer;
    buffer.init_static_with_data(data, binding_point);
    return buffer;
}

auto UniformBuffer::init_static_with_data(auto&& data) -> void
{
    _buffer.init_static_with_data(data);
}

auto UniformBuffer::init_static_with_data(auto&& data, u32 binding_point) -> void
{
    init_static_with_data(data);
    bind(binding_point);
}

auto UniformBuffer::init_static_with_data(std::ranges::contiguous_range auto&& data) -> void
{
    _buffer.init_static_with_data(data);
}

auto UniformBuffer::init_static_with_data(std::ranges::contiguous_range auto&& data, u32 binding_point) -> void
{
    init_static_with_data(data);
    bind(binding_point);
}

auto UniformBuffer::buffer_data(auto&& data, u32 offset) -> u32
{
    return _buffer.buffer_data(data, offset);
}

auto UniformBuffer::buffer_data(std::ranges::contiguous_range auto&& data, u32 offset) -> u32
{
    return _buffer.buffer_data(data, offset);
}

// --------------------------- ShaderStorageBuffer ---------------------------

auto ShaderStorageBuffer::create_static_with_data(auto&& data) -> ShaderStorageBuffer
{
    ShaderStorageBuffer buffer;
    buffer.init_static_with_data(data);
    return buffer;
}

auto ShaderStorageBuffer::create_static_with_data(auto&& data, u32 binding_point) -> ShaderStorageBuffer
{
    ShaderStorageBuffer buffer;
    buffer.init_static_with_data(data, binding_point);
    return buffer;
}

auto ShaderStorageBuffer::create_static_with_data(std::ranges::contiguous_range auto&& data) -> ShaderStorageBuffer
{
    ShaderStorageBuffer buffer;
    buffer.init_static_with_data(data);
    return buffer;
}

auto ShaderStorageBuffer::create_static_with_data(std::ranges::contiguous_range auto&& data, u32 binding_point)
    -> ShaderStorageBuffer
{
    ShaderStorageBuffer buffer;
    buffer.init_static_with_data(data, binding_point);
    return buffer;
}

auto ShaderStorageBuffer::create_dynamic_with_data(auto&& data, BufferUsage usage) -> ShaderStorageBuffer
{
    ShaderStorageBuffer buffer;
    buffer.init_dynamic_with_data(data, usage);
    return buffer;
}

auto ShaderStorageBuffer::create_dynamic_with_data(auto&& data, u32 binding_point, BufferUsage usage)
    -> ShaderStorageBuffer
{
    ShaderStorageBuffer buffer;
    buffer.init_dynamic_with_data(data, binding_point, usage);
    return buffer;
}

auto ShaderStorageBuffer::create_dynamic_with_data(std::ranges::contiguous_range auto&& data, BufferUsage usage)
    -> ShaderStorageBuffer
{
    ShaderStorageBuffer buffer;
    buffer.init_dynamic_with_data(data, usage);
    return buffer;
}

auto ShaderStorageBuffer::create_dynamic_with_data(std::ranges::contiguous_range auto&& data, u32 binding_point,
                                                   BufferUsage usage) -> ShaderStorageBuffer
{
    ShaderStorageBuffer buffer;
    buffer.init_dynamic_with_data(data, binding_point, usage);
    return buffer;
}

auto ShaderStorageBuffer::init_static_with_data(auto&& data) -> void
{
    _buffer.init_static_with_data(data);
}

auto ShaderStorageBuffer::init_static_with_data(auto&& data, u32 binding_point) -> void
{
    init_static_with_data(data);
    bind(binding_point);
}

auto ShaderStorageBuffer::init_static_with_data(std::ranges::contiguous_range auto&& data) -> void
{
    _buffer.init_static_with_data(data);
}

auto ShaderStorageBuffer::init_static_with_data(std::ranges::contiguous_range auto&& data, u32 binding_point) -> void
{
    init_static_with_data(data);
    bind(binding_point);
}

auto ShaderStorageBuffer::init_dynamic_with_data(auto&& data, BufferUsage usage) -> void
{
    _buffer.init_dynamic_with_data(data, usage);
}

auto ShaderStorageBuffer::init_dynamic_with_data(auto&& data, u32 binding_point, BufferUsage usage) -> void
{
    init_dynamic_with_data(data, usage);
    bind(binding_point);
}

auto ShaderStorageBuffer::init_dynamic_with_data(std::ranges::contiguous_range auto&& data, BufferUsage usage) -> void
{
    _buffer.init_dynamic_with_data(data, usage);
}

auto ShaderStorageBuffer::init_dynamic_with_data(std::ranges::contiguous_range auto&& data, u32 binding_point,
                                                 BufferUsage usage) -> void
{
    init_dynamic_with_data(data, usage);
    bind(binding_point);
}

auto ShaderStorageBuffer::buffer_data(auto&& data, u32 offset) -> u32
{
    return _buffer.buffer_data(data, offset);
}

auto ShaderStorageBuffer::buffer_data(std::ranges::contiguous_range auto&& data, u32 offset) -> u32
{
    return _buffer.buffer_data(data, offset);
}

template<GlBuffer DstBuffer, GlBuffer SrcBuffer>
    requires(!std::is_const_v<std::remove_reference_t<DstBuffer>>)
auto copy_buffer_data(DstBuffer& dst, SrcBuffer& src, u32 size_bytes, u32 dst_offset, u32 src_offset) -> u32
{
    ZTH_ASSERT(dst.size_bytes() >= dst_offset + size_bytes);
    ZTH_ASSERT(src.size_bytes() >= src_offset + size_bytes);
    glCopyNamedBufferSubData(src.native_handle(), dst.native_handle(), src_offset, dst_offset, size_bytes);

    return size_bytes;
}

template<GlBuffer DstBuffer, GlBuffer SrcBuffer>
    requires(!std::is_const_v<std::remove_reference_t<DstBuffer>>)
auto copy_buffer_data_to_dynamic_buffer(DstBuffer& dst, SrcBuffer& src, u32 size_bytes, u32 dst_offset, u32 src_offset)
    -> u32
{
    dst.resize_to_at_least(dst_offset + size_bytes);
    return copy_buffer_data(dst, src, size_bytes, dst_offset, src_offset);
}

} // namespace zth::gl
