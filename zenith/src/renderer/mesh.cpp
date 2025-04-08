#include "zenith/renderer/mesh.hpp"

#include "zenith/renderer/renderer.hpp"

namespace zth {

Mesh::Mesh(const void* vertex_data, usize vertex_data_size_bytes, const gl::VertexLayout& vertex_layout,
           const void* index_data, usize index_data_size_bytes, gl::DataType index_data_type)
    : _vertex_buffer(gl::VertexBuffer::create_static_with_data(vertex_data, static_cast<u32>(vertex_data_size_bytes),
                                                               vertex_layout)),
      _index_buffer(gl::IndexBuffer::create_static_with_data(index_data, static_cast<u32>(index_data_size_bytes),
                                                             index_data_type)),
      _vertex_array(_vertex_buffer, _index_buffer, Renderer::instance_buffer())
{}

Mesh::Mesh(const Mesh& other)
    : _vertex_buffer(other._vertex_buffer), _index_buffer(other._index_buffer),
      _vertex_array(_vertex_buffer, _index_buffer, Renderer::instance_buffer())
{}

auto Mesh::operator=(const Mesh& other) -> Mesh&
{
    if (this == &other)
        return *this;

    _vertex_buffer = other._vertex_buffer;
    _index_buffer = other._index_buffer;
    _vertex_array = gl::VertexArray{ _vertex_buffer, _index_buffer, Renderer::instance_buffer() };

    return *this;
}

Mesh::Mesh(Mesh&& other) noexcept
    : _vertex_buffer(std::move(other._vertex_buffer)), _index_buffer(std::move(other._index_buffer)),
      _vertex_array(std::move(other._vertex_array))
{
    // Make sure that the references in the vertex array are set again after moving the buffers. We don't have to rebind
    // the layout.
    _vertex_array.bind_vertex_buffer(_vertex_buffer);
    _vertex_array.bind_index_buffer(_index_buffer);
}

auto Mesh::operator=(Mesh&& other) noexcept -> Mesh&
{
    _vertex_buffer = std::move(other._vertex_buffer);
    _index_buffer = std::move(other._index_buffer);
    _vertex_array = std::move(other._vertex_array);

    // Make sure that the references in the vertex array are set again after moving the buffers. We don't have to rebind
    // the layout.
    _vertex_array.bind_vertex_buffer(_vertex_buffer);
    _vertex_array.bind_index_buffer(_index_buffer);

    return *this;
}

} // namespace zth
