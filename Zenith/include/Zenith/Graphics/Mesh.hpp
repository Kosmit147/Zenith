#pragma once

#include <ranges>

#include "Zenith/Platform/OpenGl/GlBuffer.hpp"
#include "Zenith/Platform/OpenGl/VertexArray.hpp"
#include "Zenith/Utility/Macros.hpp"

namespace zth {

class Mesh
{
public:
    explicit Mesh(std::ranges::contiguous_range auto&& vertex_data, std::ranges::contiguous_range auto&& index_data)
        : _vertex_buffer(VertexBuffer::create_static(vertex_data)),
          _index_buffer(IndexBuffer::create_static(index_data)), _vertex_array(_vertex_buffer, _index_buffer)
    {}

    ZTH_NO_COPY(Mesh)

    Mesh(Mesh&& other) noexcept;
    auto operator=(Mesh&& other) noexcept -> Mesh&;

    ~Mesh() = default;

    [[nodiscard]] auto vertex_array() const -> auto& { return _vertex_array; }
    [[nodiscard]] auto vertex_buffer() const -> auto& { return _vertex_buffer; }
    [[nodiscard]] auto index_buffer() const -> auto& { return _index_buffer; }

private:
    VertexBuffer _vertex_buffer;
    IndexBuffer _index_buffer;
    VertexArray _vertex_array;
};

} // namespace zth
