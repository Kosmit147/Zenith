#pragma once

#include <ranges>
#include <span>

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

    explicit Mesh(std::span<const byte> vertex_data, const gl::VertexLayout& vertex_layout,
                  std::span<const byte> index_data, gl::DataType index_data_type);

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
    : Mesh(std::as_bytes(std::span{ vertex_data }), vertex_layout, std::as_bytes(std::span{ index_data }),
           gl::IndexBuffer::derive_index_data_type<IndexData>())
{}

} // namespace zth
