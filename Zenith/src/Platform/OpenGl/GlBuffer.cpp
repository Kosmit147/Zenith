#include "Zenith/Platform/OpenGl/GlBuffer.hpp"

namespace zth {

GlBuffer::GlBuffer()
{
    create();
}

GlBuffer::GlBuffer(GlBuffer&& other) noexcept : _id(other._id), _size(other._size), _size_bytes(other._size_bytes)
{
    other._id = GL_NONE;
    other._size = 0;
    other._size_bytes = 0;
}

auto GlBuffer::operator=(GlBuffer&& other) noexcept -> GlBuffer&
{
    destroy();

    _id = other._id;
    _size = other._size;
    _size_bytes = other._size_bytes;

    other._id = GL_NONE;
    other._size = 0;
    other._size_bytes = 0;

    return *this;
}

GlBuffer::~GlBuffer()
{
    destroy();
}

auto GlBuffer::free() -> void
{
    destroy();

    _id = 0;
    _size = 0;
    _size_bytes = 0;
}

auto GlBuffer::create() -> void
{
    glCreateBuffers(1, &_id);
}

auto GlBuffer::destroy() const -> void
{
    glDeleteBuffers(1, &_id);
}

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
    : GlBuffer(std::move(other)), _layout(std::move(other._layout)), _stride(other._stride)
{
    other._stride = 0;
}

auto VertexBuffer::operator=(VertexBuffer&& other) noexcept -> VertexBuffer&
{
    GlBuffer::operator=(std::move(other));

    _layout = std::move(other._layout);
    _stride = other._stride;

    other._stride = 0;

    return *this;
}

auto VertexBuffer::bind() const -> void
{
    glBindBuffer(GL_ARRAY_BUFFER, native_handle());
}

auto VertexBuffer::unbind() const -> void
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::free()
{
    GlBuffer::free();
    _layout.clear();
    _stride = 0;
}

IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept : GlBuffer(std::move(other)), _index_type(other._index_type)
{
    other._index_type = GL_NONE;
}

auto IndexBuffer::operator=(IndexBuffer&& other) noexcept -> IndexBuffer&
{
    GlBuffer::operator=(std::move(other));
    
    _index_type = other._index_type;

    other._index_type = GL_NONE;

    return *this;
}

auto IndexBuffer::bind() const -> void
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, native_handle());
}

auto IndexBuffer::unbind() const -> void
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::free()
{
    GlBuffer::free();
    _index_type = GL_NONE;
}

} // namespace zth
