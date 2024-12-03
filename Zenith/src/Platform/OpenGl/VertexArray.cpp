#include "Zenith/Platform/OpenGl/VertexArray.hpp"

#include "Zenith/Core/Assert.hpp"
#include "Zenith/Platform/OpenGl/GlBuffer.hpp"
#include "Zenith/Platform/OpenGl/VertexLayout.hpp"

namespace zth {

VertexArray::VertexArray()
{
    create();
}

VertexArray::VertexArray(const VertexBuffer& vertex_buffer, const IndexBuffer& index_buffer) : VertexArray()
{
    bind_vertex_buffer(vertex_buffer);
    bind_index_buffer(index_buffer);
}

VertexArray::~VertexArray()
{
    destroy();
}

auto VertexArray::bind() const -> void
{
    glBindVertexArray(_id);
}

auto VertexArray::unbind() const -> void
{
    glBindVertexArray(0);
}

auto VertexArray::bind_vertex_buffer(const VertexBuffer& vertex_buffer) -> void
{
    _vertex_buffer = &vertex_buffer;
    glVertexArrayVertexBuffer(_id, 0, _vertex_buffer->native_handle(), 0, _vertex_buffer->stride());
    bind_layout(_vertex_buffer->layout());
}

auto VertexArray::bind_index_buffer(const IndexBuffer& index_buffer) -> void
{
    _index_buffer = &index_buffer;
    glVertexArrayElementBuffer(_id, _index_buffer->native_handle());
}

auto VertexArray::count() const -> GLsizei
{
    ZTH_ASSERT(_index_buffer != nullptr);
    return _index_buffer->size();
}

auto VertexArray::index_type() const -> GLenum
{
    ZTH_ASSERT(_index_buffer != nullptr);
    return _index_buffer->index_type();
}

auto VertexArray::create() -> void
{
    glCreateVertexArrays(1, &_id);
}

auto VertexArray::destroy() const -> void
{
    glDeleteVertexArrays(1, &_id);
}

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
