#pragma once

#include <ranges>
#include <span>

#include "zenith/core/typedefs.hpp"
#include "zenith/gl/buffer.hpp"
#include "zenith/gl/util.hpp"
#include "zenith/gl/vertex_array.hpp"
#include "zenith/stl/span.hpp"

namespace zth {

// Every mesh's vertex array gets implicitly bound to the renderer's instance buffer.
class Mesh
{
public:
    explicit Mesh() = default;

    explicit Mesh(std::span<const byte> vertex_data, const gl::VertexLayout& vertex_layout,
                  std::span<const byte> index_data, gl::DataType indexing_data_type);

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

// QuadMesh is a mesh made up of quads (sets of two triangles). There's no need to provide indices. The order of
// vertices for a single quad should be counter-clockwise starting from the top-left vertex. The number of vertices that
// gets drawn is the number of vertices in the vertex buffer (its size divided by its stride). Every mesh's vertex array
// gets implicitly bound to the renderer's instance buffer.
class QuadMesh
{
public:
    explicit QuadMesh() = default;

    explicit QuadMesh(std::span<const byte> vertex_data, const gl::VertexLayout& vertex_layout);

    template<std::ranges::contiguous_range VertexData>
    explicit QuadMesh(const VertexData& vertex_data, const gl::VertexLayout& vertex_layout);

    QuadMesh(const QuadMesh& other);
    auto operator=(const QuadMesh& other) -> QuadMesh&;

    QuadMesh(QuadMesh&& other) noexcept;
    auto operator=(QuadMesh&& other) noexcept -> QuadMesh&;

    ~QuadMesh() = default;

    [[nodiscard]] auto vertex_array(this auto&& self) -> auto& { return self._vertex_array; }
    [[nodiscard]] auto vertex_buffer(this auto&& self) -> auto& { return self._vertex_buffer; }

private:
    gl::VertexBuffer _vertex_buffer;
    gl::VertexArray _vertex_array;
};

template<std::ranges::contiguous_range VertexData, std::ranges::contiguous_range IndexData>
Mesh::Mesh(const VertexData& vertex_data, const gl::VertexLayout& vertex_layout, const IndexData& index_data)
    : Mesh(make_dynamic_span(std::as_bytes(std::span{ vertex_data })), vertex_layout,
           make_dynamic_span(std::as_bytes(std::span{ index_data })),
           gl::IndexBuffer::derive_indexing_data_type<IndexData>())
{}

template<std::ranges::contiguous_range VertexData>
QuadMesh::QuadMesh(const VertexData& vertex_data, const gl::VertexLayout& vertex_layout)
    : QuadMesh(make_dynamic_span(std::as_bytes(std::span{ vertex_data })), vertex_layout)
{}

} // namespace zth
