#pragma once

#include <ranges>

#include "zenith/core/typedefs.hpp"
#include "zenith/gl/buffer.hpp"
#include "zenith/gl/util.hpp"
#include "zenith/gl/vertex_array.hpp"

namespace zth {

// Every mesh's vertex array gets implicitly bound to the renderer's instance buffer.
class Mesh
{
public:
    explicit Mesh() = default;

    explicit Mesh(const void* vertex_data, usize vertex_data_size_bytes, const gl::VertexLayout& vertex_layout,
                  const void* index_data, usize index_data_size_bytes, gl::DataType index_data_type);

    template<std::ranges::contiguous_range VertexData, std::ranges::contiguous_range IndexData>
    explicit Mesh(const VertexData& vertex_data, const gl::VertexLayout& vertex_layout, const IndexData& index_data);

    Mesh(const Mesh& other);
    auto operator=(const Mesh& other) -> Mesh&;

    Mesh(Mesh&& other) noexcept;
    auto operator=(Mesh&& other) noexcept -> Mesh&;

    ~Mesh() = default;

    [[nodiscard]] auto vertex_array(this auto&& self) -> auto& { return self._vertex_array; }
    [[nodiscard]] auto vertex_buffer(this auto&& self) -> auto& { return self._vertex_buffer; }
    [[nodiscard]] auto index_buffer(this auto&& self) -> auto& { return self._index_buffer; }

private:
    gl::VertexBuffer _vertex_buffer;
    gl::IndexBuffer _index_buffer;
    gl::VertexArray _vertex_array;
};

template<std::ranges::contiguous_range VertexData, std::ranges::contiguous_range IndexData>
Mesh::Mesh(const VertexData& vertex_data, const gl::VertexLayout& vertex_layout, const IndexData& index_data)
    : Mesh(std::data(vertex_data), std::size(vertex_data) * sizeof(std::ranges::range_value_t<VertexData>),
           vertex_layout, std::data(index_data), std::size(index_data) * sizeof(std::ranges::range_value_t<IndexData>),
           gl::IndexBuffer::derive_index_data_type<IndexData>())
{}

} // namespace zth
