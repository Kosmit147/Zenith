#include "zenith/renderer/mesh.hpp"

namespace zth {

Mesh::Mesh(const void* vertex_data, usize vertex_data_size_bytes, usize vertex_stride, const void* index_data,
           usize index_data_size_bytes, gl::DataType index_data_type, const gl::VertexArrayLayout& layout)
    : _vertex_buffer(gl::VertexBuffer::create_static_with_data(vertex_data, static_cast<u32>(vertex_data_size_bytes),
                                                               static_cast<u32>(vertex_stride))),
      _index_buffer(gl::IndexBuffer::create_static_with_data(index_data, static_cast<u32>(index_data_size_bytes),
                                                             index_data_type)),
      _vertex_array(layout, _vertex_buffer, _index_buffer)
{}

Mesh::Mesh(const void* vertex_data, usize vertex_data_size_bytes, usize vertex_stride, const void* index_data,
           usize index_data_size_bytes, gl::DataType index_data_type, const gl::VertexArrayLayout& layout,
           const gl::InstanceBuffer& instance_buffer)
    : _vertex_buffer(gl::VertexBuffer::create_static_with_data(vertex_data, static_cast<u32>(vertex_data_size_bytes),
                                                               static_cast<u32>(vertex_stride))),
      _index_buffer(gl::IndexBuffer::create_static_with_data(index_data, static_cast<u32>(index_data_size_bytes),
                                                             index_data_type)),
      _vertex_array(layout, _vertex_buffer, _index_buffer, instance_buffer)
{}

Mesh::Mesh(Mesh&& other) noexcept
    : _vertex_buffer(std::move(other._vertex_buffer)), _index_buffer(std::move(other._index_buffer)),
      _vertex_array(std::move(other._vertex_array))
{
    _vertex_array.bind_vertex_buffer(_vertex_buffer);
    _vertex_array.bind_index_buffer(_index_buffer);
}

auto Mesh::operator=(Mesh&& other) noexcept -> Mesh&
{
    _vertex_buffer = std::move(other._vertex_buffer);
    _index_buffer = std::move(other._index_buffer);
    _vertex_array = std::move(other._vertex_array);

    _vertex_array.bind_vertex_buffer(_vertex_buffer);
    _vertex_array.bind_index_buffer(_index_buffer);

    return *this;
}

} // namespace zth
