#include "zenith/gl/buffer.hpp"

#include "zenith/core/assert.hpp"

namespace zth::gl {

// --------------------------- GlBuffer ---------------------------

Buffer::Buffer()
{
    create();
}

auto Buffer::create_static(usize size) -> Buffer
{
    Buffer buffer;
    buffer.init_static(size);
    return buffer;
}

auto Buffer::create_dynamic(BufferUsage usage) -> Buffer
{
    Buffer buffer;
    buffer.init_dynamic(usage);
    return buffer;
}

auto Buffer::create_dynamic(usize size, BufferUsage usage) -> Buffer
{
    Buffer buffer;
    buffer.init_dynamic(size, usage);
    return buffer;
}

Buffer::Buffer(Buffer&& other) noexcept
    : _id(other._id), _size_bytes(other._size_bytes), _state(other._state), _usage(other._usage)
{
    other._id = GL_NONE;
    other._size_bytes = 0;
    other._state = BufferState::Uninitialized;
    other._usage = std::nullopt;
}

auto Buffer::operator=(Buffer&& other) noexcept -> Buffer&
{
    destroy();

    _id = other._id;
    _size_bytes = other._size_bytes;
    _state = other._state;
    _usage = other._usage;

    other._id = GL_NONE;
    other._size_bytes = 0;
    other._state = BufferState::Uninitialized;
    other._usage = std::nullopt;

    return *this;
}

Buffer::~Buffer()
{
    destroy();
}

auto Buffer::init_static(usize size) -> void
{
    ZTH_ASSERT(_state == BufferState::Uninitialized);
    ZTH_ASSERT(size != 0); // can't initialize a static buffer with size 0

    _size_bytes = static_cast<GLsizei>(size);
    glNamedBufferStorage(_id, _size_bytes, nullptr, GL_DYNAMIC_STORAGE_BIT);

    _state = BufferState::InitializedStatic;
}

auto Buffer::init_dynamic(BufferUsage usage) -> void
{
    ZTH_ASSERT(_state != BufferState::InitializedStatic);

    _size_bytes = 0;
    _usage = usage;
    glNamedBufferData(_id, _size_bytes, nullptr, to_gl_enum(*_usage));

    _state = BufferState::InitializedDynamic;
}

auto Buffer::init_dynamic(usize size, BufferUsage usage) -> void
{
    ZTH_ASSERT(_state != BufferState::InitializedStatic);

    _size_bytes = static_cast<GLsizei>(size);
    _usage = usage;
    glNamedBufferData(_id, _size_bytes, nullptr, to_gl_enum(*_usage));

    _state = BufferState::InitializedDynamic;
}

auto Buffer::buffer_data(const void* data, usize offset, usize data_size_bytes) -> void
{
    ZTH_ASSERT(_state != BufferState::Uninitialized);

    if (_state == BufferState::InitializedStatic)
    {
        ZTH_ASSERT(static_cast<GLsizei>(offset + data_size_bytes) <= _size_bytes);
        glNamedBufferSubData(_id, static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(data_size_bytes), data);
    }
    else if (_state == BufferState::InitializedDynamic)
    {
        if (static_cast<GLsizei>(offset + data_size_bytes) > _size_bytes)
            resize_to_at_least(static_cast<GLsizei>(offset + data_size_bytes));

        glNamedBufferSubData(_id, static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(data_size_bytes), data);
    }
    else
    {
        ZTH_ASSERT(false);
    }
}

auto Buffer::create() -> void
{
    glCreateBuffers(1, &_id);
}

auto Buffer::destroy() -> void
{
    glDeleteBuffers(1, &_id);
}

auto Buffer::resize_to_at_least(GLsizei min_size_bytes) -> void
{
    ZTH_ASSERT(_state == BufferState::InitializedDynamic);

    if (min_size_bytes <= _size_bytes)
        return;

    auto old_size = _size_bytes;
    auto new_size = old_size * 2 + 1;

    while (new_size < min_size_bytes)
        new_size *= 2;

    if (old_size != 0)
    {
        // need to copy the buffer's contents to newly allocated memory

        Buffer tmp_buffer = create_static(old_size);
        glCopyNamedBufferSubData(_id, tmp_buffer._id, 0, 0, old_size);

        ZTH_ASSERT(_usage.has_value());
        glNamedBufferData(_id, new_size, nullptr, to_gl_enum(*_usage));
        glCopyNamedBufferSubData(tmp_buffer._id, _id, 0, 0, old_size);
    }
    else
    {
        ZTH_ASSERT(_usage.has_value());
        glNamedBufferData(_id, new_size, nullptr, to_gl_enum(*_usage));
    }

    _size_bytes = new_size;
}

// --------------------------- VertexBuffer ---------------------------

auto VertexBuffer::create_static(usize size) -> VertexBuffer
{
    VertexBuffer buffer;
    buffer.init_static(size);
    return buffer;
}

auto VertexBuffer::create_dynamic(BufferUsage usage) -> VertexBuffer
{
    VertexBuffer buffer;
    buffer.init_dynamic(usage);
    return buffer;
}

auto VertexBuffer::create_dynamic(usize size, BufferUsage usage) -> VertexBuffer
{
    VertexBuffer buffer;
    buffer.init_dynamic(size, usage);
    return buffer;
}

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept : _buffer(std::move(other._buffer)), _stride(other._stride)
{
    other._stride = 0;
}

auto VertexBuffer::operator=(VertexBuffer&& other) noexcept -> VertexBuffer&
{
    _buffer = std::move(other._buffer);
    _stride = other._stride;

    other._stride = 0;

    return *this;
}

auto VertexBuffer::init_static(usize size) -> void
{
    _buffer.init_static(size);
}

auto VertexBuffer::init_dynamic(BufferUsage usage) -> void
{
    _buffer.init_dynamic(usage);
}

auto VertexBuffer::init_dynamic(usize size, BufferUsage usage) -> void
{
    _buffer.init_dynamic(size, usage);
}

auto VertexBuffer::bind() const -> void
{
    glBindBuffer(GL_ARRAY_BUFFER, native_handle());
}

auto VertexBuffer::unbind() -> void
{
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
}

auto VertexBuffer::set_stride(GLsizei stride) -> void
{
    _stride = stride;
}

// --------------------------- IndexBuffer ---------------------------

auto IndexBuffer::create_static(usize size) -> IndexBuffer
{
    IndexBuffer buffer;
    buffer.init_static(size);
    return buffer;
}

auto IndexBuffer::create_dynamic(BufferUsage usage) -> IndexBuffer
{
    IndexBuffer buffer;
    buffer.init_dynamic(usage);
    return buffer;
}

auto IndexBuffer::create_dynamic(usize size, BufferUsage usage) -> IndexBuffer
{
    IndexBuffer buffer;
    buffer.init_dynamic(size, usage);
    return buffer;
}

IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
    : _buffer(std::move(other._buffer)), _index_type(other._index_type), _size(other._size)
{
    other._index_type = GL_NONE;
    other._size = 0;
}

auto IndexBuffer::operator=(IndexBuffer&& other) noexcept -> IndexBuffer&
{
    _buffer = std::move(other._buffer);
    _index_type = other._index_type;
    _size = other._size;

    other._index_type = GL_NONE;
    other._size = 0;

    return *this;
}

auto IndexBuffer::init_static(usize size) -> void
{
    _buffer.init_static(size);
}

auto IndexBuffer::init_dynamic(BufferUsage usage) -> void
{
    _buffer.init_dynamic(usage);
}

auto IndexBuffer::init_dynamic(usize size, BufferUsage usage) -> void
{
    _buffer.init_dynamic(size, usage);
}

auto IndexBuffer::bind() const -> void
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, native_handle());
}

auto IndexBuffer::unbind() -> void
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);
}

// --------------------------- InstanceBuffer ---------------------------

auto InstanceBuffer::create_static(usize size) -> InstanceBuffer
{
    InstanceBuffer buffer;
    buffer.init_static(size);
    return buffer;
}

auto InstanceBuffer::create_dynamic(BufferUsage usage) -> InstanceBuffer
{
    InstanceBuffer buffer;
    buffer.init_dynamic(usage);
    return buffer;
}

auto InstanceBuffer::create_dynamic(usize size, BufferUsage usage) -> InstanceBuffer
{
    InstanceBuffer buffer;
    buffer.init_dynamic(size, usage);
    return buffer;
}

// --------------------------- UniformBuffer ---------------------------

auto UniformBuffer::create_static(usize size) -> UniformBuffer
{
    UniformBuffer buffer;
    buffer.init_static(size);
    return buffer;
}

auto UniformBuffer::create_static(usize size, u32 binding_index) -> UniformBuffer
{
    UniformBuffer buffer;
    buffer.init_static(size, binding_index);
    return buffer;
}

auto UniformBuffer::init_static(usize size) -> void
{
    _buffer.init_static(size);
}

auto UniformBuffer::init_static(usize size, u32 binding_index) -> void
{
    _buffer.init_static(size);
    set_binding_index(binding_index);
}

auto UniformBuffer::buffer_data(const void* data, usize offset, usize size_bytes) -> void
{
    _buffer.buffer_data(data, offset, size_bytes);
}

auto UniformBuffer::bind() const -> void
{
    glBindBuffer(GL_UNIFORM_BUFFER, native_handle());
}

auto UniformBuffer::unbind() -> void
{
    glBindBuffer(GL_UNIFORM_BUFFER, GL_NONE);
}

auto UniformBuffer::set_binding_index(u32 index) const -> void
{
    glBindBufferBase(GL_UNIFORM_BUFFER, index, native_handle());
}

} // namespace zth::gl
