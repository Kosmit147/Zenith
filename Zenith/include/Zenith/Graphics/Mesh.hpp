#pragma once

#include <ranges>

#include "Zenith/Platform/OpenGl/GlBuffer.hpp"
#include "Zenith/Platform/OpenGl/VertexArray.hpp"

namespace zth {

class Mesh
{
public:
    explicit Mesh(std::ranges::contiguous_range auto&& vertex_data, std::ranges::contiguous_range auto&& index_data)
    {
        (void)vertex_data;
        (void)index_data;
    }

private:
    VertexArray _vertex_array;
    VertexBuffer _vertex_buffer;
    IndexBuffer _index_buffer;
};

} // namespace zth
