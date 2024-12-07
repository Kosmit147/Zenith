#include "Zenith/Platform/OpenGl/VertexArray.hpp"

#include "Zenith/Core/Assert.hpp"
#include "Zenith/Graphics/Renderer.hpp"
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

VertexArray::VertexArray(const VertexArray& other) : VertexArray()
{
    bind_vertex_buffer(other.vertex_buffer());
    bind_index_buffer(other.index_buffer());
}

auto VertexArray::operator=(const VertexArray& other) -> VertexArray&
{
    if (this == &other)
        return *this;

    destroy();
    create();

    bind_vertex_buffer(other.vertex_buffer());
    bind_index_buffer(other.index_buffer());

    return *this;
}

VertexArray::VertexArray(VertexArray&& other) noexcept
    : _id(other._id), _vertex_buffer(other._vertex_buffer), _index_buffer(other._index_buffer)
{
    other._id = GL_NONE;
    other._vertex_buffer = nullptr;
    other._index_buffer = nullptr;
}

auto VertexArray::operator=(VertexArray&& other) noexcept -> VertexArray&
{
    destroy();

    _id = other._id;
    _vertex_buffer = other._vertex_buffer;
    _index_buffer = other._index_buffer;

    other._id = GL_NONE;
    other._vertex_buffer = nullptr;
    other._index_buffer = nullptr;

    return *this;
}

VertexArray::~VertexArray()
{
    destroy();
}

auto VertexArray::draw() const -> void
{
    Renderer::draw(*this);
}

auto VertexArray::bind() const -> void
{
    glBindVertexArray(_id);
}

auto VertexArray::unbind() -> void
{
    glBindVertexArray(GL_NONE);
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
