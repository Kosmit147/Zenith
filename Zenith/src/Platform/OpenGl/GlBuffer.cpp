#include "Zenith/Platform/OpenGl/GlBuffer.hpp"

namespace zth {

GlBuffer::GlBuffer()
{
    create();
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
