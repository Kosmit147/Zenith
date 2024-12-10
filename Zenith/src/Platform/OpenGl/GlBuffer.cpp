#include "Zenith/Platform/OpenGl/GlBuffer.hpp"

namespace zth {

GlBuffer::GlBuffer()
{
    create();
}

GlBuffer::GlBuffer(GLsizei size) : GlBuffer()
{
    _size_bytes = size;
    glNamedBufferStorage(_id, _size_bytes, nullptr, GL_DYNAMIC_STORAGE_BIT);
}

GlBuffer::GlBuffer(GlBuffer&& other) noexcept : _id(other._id), _size_bytes(other._size_bytes)
{
    other._id = GL_NONE;
    other._size_bytes = 0;
}

auto GlBuffer::operator=(GlBuffer&& other) noexcept -> GlBuffer&
{
    destroy();

    _id = other._id;
    _size_bytes = other._size_bytes;

    other._id = GL_NONE;
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
    : GlBuffer(std::move(other)), _layout(std::move(other._layout)), _size(other._size), _stride(other._stride)
{
    other._size = 0;
    other._stride = 0;
}

auto VertexBuffer::operator=(VertexBuffer&& other) noexcept -> VertexBuffer&
{
    GlBuffer::operator=(std::move(other));

    _layout = std::move(other._layout);
    _size = other._size;
    _stride = other._stride;

    other._size = 0;
    other._stride = 0;

    return *this;
}

auto VertexBuffer::bind() const -> void
{
    glBindBuffer(GL_ARRAY_BUFFER, native_handle());
}

auto VertexBuffer::unbind() const -> void
{
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
}

void VertexBuffer::free()
{
    GlBuffer::free();
    _layout.clear();
    _size = 0;
    _stride = 0;
}

IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
    : GlBuffer(std::move(other)), _index_type(other._index_type), _size(other._size)
{
    other._index_type = GL_NONE;
    other._size = 0;
}

auto IndexBuffer::operator=(IndexBuffer&& other) noexcept -> IndexBuffer&
{
    GlBuffer::operator=(std::move(other));
    
    _index_type = other._index_type;
    _size = other._size;

    other._index_type = GL_NONE;
    other._size = 0;

    return *this;
}

auto IndexBuffer::bind() const -> void
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, native_handle());
}

auto IndexBuffer::unbind() const -> void
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);
}

void IndexBuffer::free()
{
    GlBuffer::free();
    _index_type = GL_NONE;
    _size = 0;
}

auto UniformBuffer::buffer_sub_data(const void* data, GLintptr offset, GLsizeiptr size_bytes) const -> void
{
    ZTH_ASSERT(size_bytes + offset <= this->size_bytes());
    glNamedBufferSubData(native_handle(), offset, size_bytes, data);
}

auto UniformBuffer::bind() const -> void
{
    glBindBuffer(GL_UNIFORM_BUFFER, native_handle());
}

auto UniformBuffer::unbind() const -> void
{
    glBindBuffer(GL_UNIFORM_BUFFER, GL_NONE);
}

auto UniformBuffer::set_binding_index(GLuint index) const -> void
{
    glBindBufferBase(GL_UNIFORM_BUFFER, index, native_handle());
}

} // namespace zth
