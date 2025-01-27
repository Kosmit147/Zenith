#pragma once

#include <ranges>

#include "zenith/gl/buffer.hpp"
#include "zenith/gl/vertex_array.hpp"
#include "zenith/util/macros.hpp"

namespace zth {

class Mesh
{
public:
    explicit Mesh(std::ranges::contiguous_range auto&& vertex_data, std::ranges::contiguous_range auto&& index_data);
    explicit Mesh(std::ranges::contiguous_range auto&& vertex_data, std::ranges::contiguous_range auto&& index_data,
                  const gl::VertexArrayLayout& layout);
    explicit Mesh(std::ranges::contiguous_range auto&& vertex_data, std::ranges::contiguous_range auto&& index_data,
                  const gl::VertexArrayLayout& layout, const gl::InstanceBuffer& instance_buffer);

    ZTH_NO_COPY(Mesh)

    Mesh(Mesh&& other) noexcept;
    auto operator=(Mesh&& other) noexcept -> Mesh&;

    ~Mesh() = default;

    [[nodiscard]] auto vertex_array() -> auto& { return _vertex_array; }
    [[nodiscard]] auto vertex_array() const -> auto& { return _vertex_array; }
    [[nodiscard]] auto vertex_buffer() -> auto& { return _vertex_buffer; }
    [[nodiscard]] auto vertex_buffer() const -> auto& { return _vertex_buffer; }
    [[nodiscard]] auto index_buffer() -> auto& { return _index_buffer; }
    [[nodiscard]] auto index_buffer() const -> auto& { return _index_buffer; }

private:
    gl::VertexBuffer _vertex_buffer;
    gl::IndexBuffer _index_buffer;
    gl::VertexArray _vertex_array;
};

Mesh::Mesh(std::ranges::contiguous_range auto&& vertex_data, std::ranges::contiguous_range auto&& index_data)
    : _vertex_buffer(gl::VertexBuffer::create_static(vertex_data)),
      _index_buffer(gl::IndexBuffer::create_static(index_data)),
      _vertex_array(gl::VertexArrayLayout::from_vertex_data<decltype(vertex_data)>(), _vertex_buffer, _index_buffer)
{}

Mesh::Mesh(std::ranges::contiguous_range auto&& vertex_data, std::ranges::contiguous_range auto&& index_data,
           const gl::VertexArrayLayout& layout)
    : _vertex_buffer(gl::VertexBuffer::create_static(vertex_data)),
      _index_buffer(gl::IndexBuffer::create_static(index_data)), _vertex_array(layout, _vertex_buffer, _index_buffer)
{}

Mesh::Mesh(std::ranges::contiguous_range auto&& vertex_data, std::ranges::contiguous_range auto&& index_data,
           const gl::VertexArrayLayout& layout, const gl::InstanceBuffer& instance_buffer)
    : _vertex_buffer(gl::VertexBuffer::create_static(vertex_data)),
      _index_buffer(gl::IndexBuffer::create_static(index_data)),
      _vertex_array(layout, _vertex_buffer, _index_buffer, instance_buffer)
{}

} // namespace zth
