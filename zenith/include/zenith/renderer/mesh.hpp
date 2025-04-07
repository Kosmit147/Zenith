#pragma once

#include <ranges>

#include "zenith/core/typedefs.hpp"
#include "zenith/gl/buffer.hpp"
#include "zenith/gl/util.hpp"
#include "zenith/gl/vertex_array.hpp"

namespace zth {

class Mesh
{
public:
    explicit Mesh(const void* vertex_data, usize vertex_data_size_bytes, usize vertex_stride, const void* index_data,
                  usize index_data_size_bytes, gl::DataType index_data_type, const gl::VertexArrayLayout& layout);

    explicit Mesh(const void* vertex_data, usize vertex_data_size_bytes, usize vertex_stride, const void* index_data,
                  usize index_data_size_bytes, gl::DataType index_data_type, const gl::VertexArrayLayout& layout,
                  const gl::InstanceBuffer& instance_buffer);

    template<std::ranges::contiguous_range VertexData, std::ranges::contiguous_range IndexData>
    explicit Mesh(const VertexData& vertex_data, const IndexData& index_data,
                  const gl::VertexArrayLayout& layout = gl::VertexArrayLayout::derive_from_vertex_data<VertexData>());

    template<std::ranges::contiguous_range VertexData, std::ranges::contiguous_range IndexData>
    explicit Mesh(const VertexData& vertex_data, const IndexData& index_data, const gl::VertexArrayLayout& layout,
                  const gl::InstanceBuffer& instance_buffer);

    explicit Mesh(const void* vertex_data, usize vertex_data_size_bytes, usize vertex_stride, const void* index_data,
                  usize index_data_size_bytes, gl::DataType index_data_type, const gl::VertexArrayLayout& layout,
                  gl::InstanceBuffer&& instance_buffer) = delete;

    template<std::ranges::contiguous_range VertexData, std::ranges::contiguous_range IndexData>
    explicit Mesh(const VertexData& vertex_data, const IndexData& index_data, const gl::VertexArrayLayout& layout,
                  gl::InstanceBuffer&& instance_buffer) = delete;

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
Mesh::Mesh(const VertexData& vertex_data, const IndexData& index_data, const gl::VertexArrayLayout& layout)
    : _vertex_buffer(gl::VertexBuffer::create_static_with_data(vertex_data)),
      _index_buffer(gl::IndexBuffer::create_static_with_data(index_data)),
      _vertex_array(layout, _vertex_buffer, _index_buffer)
{}

template<std::ranges::contiguous_range VertexData, std::ranges::contiguous_range IndexData>
Mesh::Mesh(const VertexData& vertex_data, const IndexData& index_data, const gl::VertexArrayLayout& layout,
           const gl::InstanceBuffer& instance_buffer)
    : _vertex_buffer(gl::VertexBuffer::create_static_with_data(vertex_data)),
      _index_buffer(gl::IndexBuffer::create_static_with_data(index_data)),
      _vertex_array(layout, _vertex_buffer, _index_buffer, instance_buffer)
{}

} // namespace zth
