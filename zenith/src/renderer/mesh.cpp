#include "zenith/renderer/mesh.hpp"

#include "zenith/renderer/quad.hpp"
#include "zenith/renderer/renderer.hpp"
#include "zenith/renderer/resources/buffers.hpp"

namespace zth {

Mesh::Mesh(std::span<const byte> vertex_data, const gl::VertexLayout& vertex_layout, std::span<const byte> index_data,
           gl::DataType indexing_data_type)
    : _vertex_buffer{ gl::VertexBuffer::create_static_with_data(vertex_data, vertex_layout) },
      _index_buffer{ gl::IndexBuffer::create_static_with_data(index_data, indexing_data_type) },
      _vertex_array{ _vertex_buffer, _index_buffer, Renderer::instance_buffer() }
{}

Mesh::Mesh(const Mesh& other)
    : _vertex_buffer{ other._vertex_buffer }, _index_buffer{ other._index_buffer }, _vertex_array{ other._vertex_array }
{
    // Make sure that the references in the vertex array are set after copying the buffers. We don't have to rebind the
    // layout.
    _vertex_array.bind_vertex_buffer(_vertex_buffer);
    _vertex_array.bind_index_buffer(_index_buffer);
}

auto Mesh::operator=(const Mesh& other) -> Mesh&
{
    if (this == &other)
        return *this;

    _vertex_buffer = other._vertex_buffer;
    _index_buffer = other._index_buffer;
    _vertex_array = other._vertex_array;

    // Make sure that the references in the vertex array are set after copying the buffers. We don't have to rebind the
    // layout.
    _vertex_array.bind_vertex_buffer(_vertex_buffer);
    _vertex_array.bind_index_buffer(_index_buffer);

    return *this;
}

Mesh::Mesh(Mesh&& other) noexcept
    : _vertex_buffer{ std::move(other._vertex_buffer) }, _index_buffer{ std::move(other._index_buffer) },
      _vertex_array{ std::move(other._vertex_array) }
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

QuadMesh::QuadMesh(std::span<const byte> vertex_data, const gl::VertexLayout& vertex_layout)
    : _vertex_buffer{ gl::VertexBuffer::create_static_with_data(vertex_data, vertex_layout) },
      _vertex_array{ _vertex_buffer, buffers::quads_index_buffer(), Renderer::instance_buffer(),
                     static_cast<u32>(get_triangle_vertex_count_from_quad_vertex_count(_vertex_buffer.count())) }
{}

QuadMesh::QuadMesh(const QuadMesh& other) : _vertex_buffer{ other._vertex_buffer }, _vertex_array{ other._vertex_array }
{
    // Make sure that the references in the vertex array are set after copying the vertex buffer. We don't have to
    // rebind the layout.
    _vertex_array.bind_vertex_buffer(_vertex_buffer);

    // We don't need to rebind the index buffer.
}

auto QuadMesh::operator=(const QuadMesh& other) -> QuadMesh&
{
    if (this == &other)
        return *this;

    _vertex_buffer = other._vertex_buffer;
    _vertex_array = other._vertex_array;

    // Make sure that the references in the vertex array are set after copying the vertex buffer. We don't have to
    // rebind the layout.
    _vertex_array.bind_vertex_buffer(_vertex_buffer);

    // We don't need to rebind the index buffer.

    return *this;
}

QuadMesh::QuadMesh(QuadMesh&& other) noexcept
    : _vertex_buffer{ std::move(other._vertex_buffer) }, _vertex_array{ std::move(other._vertex_array) }
{
    // Make sure that the references in the vertex array are set again after moving the buffers. We don't have to rebind
    // the layout.
    _vertex_array.bind_vertex_buffer(_vertex_buffer);

    // We don't need to rebind the index buffer.
}

auto QuadMesh::operator=(QuadMesh&& other) noexcept -> QuadMesh&
{
    _vertex_buffer = std::move(other._vertex_buffer);
    _vertex_array = std::move(other._vertex_array);

    // Make sure that the references in the vertex array are set again after moving the buffers. We don't have to rebind
    // the layout.
    _vertex_array.bind_vertex_buffer(_vertex_buffer);

    // We don't need to rebind the index buffer.

    return *this;
}

} // namespace zth
