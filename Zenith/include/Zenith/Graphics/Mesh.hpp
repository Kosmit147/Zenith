#pragma once

#include <ranges>

#include "Zenith/Platform/OpenGl/GlBuffer.hpp"
#include "Zenith/Platform/OpenGl/VertexArray.hpp"

namespace zth {

class Mesh
{
public:
    explicit Mesh(std::ranges::contiguous_range auto&& vertex_data, BufferUsage vertex_buffer_usage,
                  std::ranges::contiguous_range auto&& index_data, BufferUsage index_buffer_usage)
        : _vertex_buffer(vertex_data, vertex_buffer_usage), _index_buffer(index_data, index_buffer_usage),
          _vertex_array(_vertex_buffer, _index_buffer)
    {}

    [[nodiscard]] auto vertex_array() const -> auto& { return _vertex_array; }
    [[nodiscard]] auto vertex_buffer() const -> auto& { return _vertex_buffer; }
    [[nodiscard]] auto index_buffer() const -> auto& { return _index_buffer; }

private:
    VertexBuffer _vertex_buffer;
    IndexBuffer _index_buffer;
    VertexArray _vertex_array;
};

} // namespace zth
