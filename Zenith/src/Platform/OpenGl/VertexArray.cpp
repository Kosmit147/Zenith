#include "Zenith/Platform/OpenGl/VertexArray.hpp"

#include "Zenith/Platform/OpenGl/VertexLayout.hpp"

namespace zth {

auto VertexArray::bind_layout(const VertexLayout& layout) const -> void
{
    GLuint index = 0;
    GLuint offset = 0;

    for (auto& elem : layout)
    {
        auto [count, type, size] = get_vertex_layout_element_info(elem);

        glEnableVertexArrayAttrib(_id, index);
        glVertexArrayAttribFormat(_id, index, count, type, GL_FALSE, offset);
        glVertexArrayAttribBinding(_id, index, 0);

        index++;
        offset += size;
    }
}

} // namespace zth
