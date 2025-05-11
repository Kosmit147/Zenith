#pragma once

#include <ranges>
#include <utility>

#include "zenith/renderer/primitives.hpp"
#include "zenith/renderer/renderer.hpp"
#include "zenith/renderer/resources/buffers.hpp"

namespace zth {

template<typename Vertex, gl::IndexingType Index>
IndexedMesh<Vertex, Index>::IndexedMesh(std::span<const Vertex> vertex_data, std::span<const Index> index_data)
    : _vertex_buffer{ gl::VertexBuffer::create_static_with_data(vertex_data) },
      _index_buffer{ gl::IndexBuffer::create_static_with_data(index_data) },
      _vertex_array{ _vertex_buffer, _index_buffer, Renderer::instance_buffer() },
      _vertices{ std::from_range_t{}, vertex_data }, _indices{ std::from_range_t{}, index_data }
{}

template<typename Vertex, gl::IndexingType Index>
IndexedMesh<Vertex, Index>::IndexedMesh(const IndexedMesh& other)
    : _vertex_buffer{ other._vertex_buffer }, _index_buffer{ other._index_buffer },
      _vertex_array{ other._vertex_array }, _vertices{ other._vertices }, _indices{ other._indices }
{
    // Make sure that the references in the vertex array are set after copying the buffers. We don't have to rebind the
    // layout.
    _vertex_array.bind_vertex_buffer(_vertex_buffer);
    _vertex_array.bind_index_buffer(_index_buffer);
}

template<typename Vertex, gl::IndexingType Index>
auto IndexedMesh<Vertex, Index>::operator=(const IndexedMesh& other) -> IndexedMesh&
{
    if (this == &other)
        return *this;

    _vertex_buffer = other._vertex_buffer;
    _index_buffer = other._index_buffer;
    _vertex_array = other._vertex_array;
    _vertices = other._vertices;
    _indices = other._indices;

    // Make sure that the references in the vertex array are set after copying the buffers. We don't have to rebind the
    // layout.
    _vertex_array.bind_vertex_buffer(_vertex_buffer);
    _vertex_array.bind_index_buffer(_index_buffer);

    return *this;
}

template<typename Vertex, gl::IndexingType Index>
IndexedMesh<Vertex, Index>::IndexedMesh(IndexedMesh&& other) noexcept
    : _vertex_buffer{ std::move(other._vertex_buffer) }, _index_buffer{ std::move(other._index_buffer) },
      _vertex_array{ std::move(other._vertex_array) }, _vertices{ std::move(other._vertices) },
      _indices{ std::move(other._indices) }
{
    // Make sure that the references in the vertex array are set again after moving the buffers. We don't have to rebind
    // the layout.
    _vertex_array.bind_vertex_buffer(_vertex_buffer);
    _vertex_array.bind_index_buffer(_index_buffer);
}

template<typename Vertex, gl::IndexingType Index>
auto IndexedMesh<Vertex, Index>::operator=(IndexedMesh&& other) noexcept -> IndexedMesh&
{
    _vertex_buffer = std::move(other._vertex_buffer);
    _index_buffer = std::move(other._index_buffer);
    _vertex_array = std::move(other._vertex_array);
    _vertices = std::move(other._vertices);
    _indices = std::move(other._indices);

    // Make sure that the references in the vertex array are set again after moving the buffers. We don't have to rebind
    // the layout.
    _vertex_array.bind_vertex_buffer(_vertex_buffer);
    _vertex_array.bind_index_buffer(_index_buffer);

    return *this;
}

template<typename Vertex>
QuadMesh<Vertex>::QuadMesh(std::span<const Vertex> vertex_data)
    : _vertex_buffer{ gl::VertexBuffer::create_static_with_data(vertex_data) },
      _vertex_array{ _vertex_buffer, buffers::quads_index_buffer(), Renderer::instance_buffer(),
                     static_cast<u32>(get_triangle_vertex_count_from_quad_vertex_count(_vertex_buffer.count())) },
      _vertices{ std::from_range_t{}, vertex_data }
{}

template<typename Vertex>
QuadMesh<Vertex>::QuadMesh(const QuadMesh& other)
    : _vertex_buffer{ other._vertex_buffer }, _vertex_array{ other._vertex_array }, _vertices{ other._vertices }
{
    // Make sure that the references in the vertex array are set after copying the vertex buffer. We don't have to
    // rebind the layout.
    _vertex_array.bind_vertex_buffer(_vertex_buffer);

    // We don't need to rebind the index buffer.
}

template<typename Vertex> auto QuadMesh<Vertex>::operator=(const QuadMesh& other) -> QuadMesh&
{
    if (this == &other)
        return *this;

    _vertex_buffer = other._vertex_buffer;
    _vertex_array = other._vertex_array;
    _vertices = other._vertices;

    // Make sure that the references in the vertex array are set after copying the vertex buffer. We don't have to
    // rebind the layout.
    _vertex_array.bind_vertex_buffer(_vertex_buffer);

    // We don't need to rebind the index buffer.

    return *this;
}

template<typename Vertex>
QuadMesh<Vertex>::QuadMesh(QuadMesh&& other) noexcept
    : _vertex_buffer{ std::move(other._vertex_buffer) }, _vertex_array{ std::move(other._vertex_array) },
      _vertices{ std::move(other._vertices) }
{
    // Make sure that the references in the vertex array are set again after moving the buffers. We don't have to rebind
    // the layout.
    _vertex_array.bind_vertex_buffer(_vertex_buffer);

    // We don't need to rebind the index buffer.
}

template<typename Vertex> auto QuadMesh<Vertex>::operator=(QuadMesh&& other) noexcept -> QuadMesh&
{
    _vertex_buffer = std::move(other._vertex_buffer);
    _vertex_array = std::move(other._vertex_array);
    _vertices = std::move(other._vertices);

    // Make sure that the references in the vertex array are set again after moving the buffers. We don't have to rebind
    // the layout.
    _vertex_array.bind_vertex_buffer(_vertex_buffer);

    // We don't need to rebind the index buffer.

    return *this;
}

template<typename Vertex> auto QuadMesh<Vertex>::index_buffer() const -> const gl::IndexBuffer&
{
    return buffers::quads_index_buffer();
}

} // namespace zth
