#pragma once

#include <memory>

#include "zenith/core/assert.hpp"

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
    init_static_with_data(std::addressof(data), sizeof(data));
}

auto Buffer::init_static_with_data(std::ranges::contiguous_range auto&& data) -> void
{
    using T = std::ranges::range_value_t<decltype(data)>;
    auto data_size_bytes = static_cast<u32>(std::size(data) * sizeof(T));
    init_static_with_data(std::data(data), data_size_bytes);
}

auto Buffer::init_dynamic_with_data(auto&& data, BufferUsage usage) -> void
{
    init_dynamic_with_data(std::addressof(data), sizeof(data), usage);
}

auto Buffer::init_dynamic_with_data(std::ranges::contiguous_range auto&& data, BufferUsage usage) -> void
{
    using T = std::ranges::range_value_t<decltype(data)>;
    auto data_size_bytes = static_cast<u32>(std::size(data) * sizeof(T));
    init_dynamic_with_data(std::data(data), data_size_bytes, usage);
}

auto Buffer::buffer_data(auto&& data, u32 offset) -> u32
{
    return buffer_data(std::addressof(data), sizeof(data), offset);
}

auto Buffer::buffer_data(std::ranges::contiguous_range auto&& data, u32 offset) -> u32
{
    using T = std::ranges::range_value_t<decltype(data)>;
    auto data_size_bytes = static_cast<u32>(std::size(data) * sizeof(T));
    return buffer_data(std::data(data), data_size_bytes, offset);
}

// --------------------------- VertexBuffer ---------------------------

auto VertexBuffer::create_static_with_data(std::ranges::contiguous_range auto&& data, const VertexLayout& layout)
    -> VertexBuffer
{
    VertexBuffer buffer;
    buffer.init_static_with_data(data, layout);
    return buffer;
}

auto VertexBuffer::create_dynamic_with_data(std::ranges::contiguous_range auto&& data, const VertexLayout& layout,
                                            BufferUsage usage) -> VertexBuffer
{
    VertexBuffer buffer;
    buffer.init_dynamic_with_data(data, layout, usage);
    return buffer;
}

auto VertexBuffer::init_static_with_data(std::ranges::contiguous_range auto&& data, const VertexLayout& layout) -> void
{
    _buffer.init_static_with_data(data);
    _layout = layout;
}

auto VertexBuffer::init_dynamic_with_data(std::ranges::contiguous_range auto&& data, const VertexLayout& layout,
                                          BufferUsage usage) -> void
{
    _buffer.init_dynamic_with_data(data, usage);
    _layout = layout;
}

auto VertexBuffer::buffer_data(std::ranges::contiguous_range auto&& data, u32 offset) -> u32
{
    return _buffer.buffer_data(data, offset);
}

// --------------------------- IndexBuffer ---------------------------

auto IndexBuffer::create_static_with_data(std::ranges::contiguous_range auto&& data) -> IndexBuffer
{
    IndexBuffer buffer;
    buffer.init_static_with_data(data);
    return buffer;
}

auto IndexBuffer::create_dynamic_with_data(std::ranges::contiguous_range auto&& data, BufferUsage usage) -> IndexBuffer
{
    IndexBuffer buffer;
    buffer.init_dynamic_with_data(data, usage);
    return buffer;
}

auto IndexBuffer::init_static_with_data(std::ranges::contiguous_range auto&& data) -> void
{
    _buffer.init_static_with_data(data);
    set_index_data_type(derive_index_data_type<decltype(data)>());
    _count = static_cast<u32>(std::size(data));
}

auto IndexBuffer::init_dynamic_with_data(std::ranges::contiguous_range auto&& data, BufferUsage usage) -> void
{
    _buffer.init_dynamic_with_data(data, usage);
    set_index_data_type(derive_index_data_type<decltype(data)>());
    _count = static_cast<u32>(std::size(data));
}

auto IndexBuffer::buffer_data(std::ranges::contiguous_range auto&& data, u32 offset) -> u32
{
    return _buffer.buffer_data(data, offset);
}

template<typename T> auto IndexBuffer::set_index_data_type() -> void
{
    set_index_data_type(to_data_type<T>);
}

template<std::ranges::contiguous_range IndexData> constexpr auto IndexBuffer::derive_index_data_type() -> DataType
{
    constexpr auto data_type = to_data_type<std::ranges::range_value_t<IndexData>>;
    static_assert(is_an_index_data_type(data_type), "data type must be ubyte, ushort or uint");
    return data_type;
}

// --------------------------- InstanceBuffer ---------------------------

auto InstanceBuffer::create_static_with_data(std::ranges::contiguous_range auto&& data, const VertexLayout& layout)
    -> InstanceBuffer
{
    InstanceBuffer buffer;
    buffer.init_static_with_data(data, layout);
    return buffer;
}

auto InstanceBuffer::create_dynamic_with_data(std::ranges::contiguous_range auto&& data, const VertexLayout& layout,
                                              BufferUsage usage) -> InstanceBuffer
{
    InstanceBuffer buffer;
    buffer.init_dynamic_with_data(data, layout, usage);
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
