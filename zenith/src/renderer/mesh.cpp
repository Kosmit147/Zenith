#include "zenith/renderer/mesh.hpp"

namespace zth {

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
