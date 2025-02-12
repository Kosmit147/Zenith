#include "zenith/gl/buffer.hpp"

#include "zenith/core/assert.hpp"

namespace zth::gl {

const BufferUsage BufferUsage::stream_draw{ BufferAccessFrequency::Stream, BufferAccessType::Draw };
const BufferUsage BufferUsage::static_draw{ BufferAccessFrequency::Static, BufferAccessType::Draw };
const BufferUsage BufferUsage::dynamic_draw{ BufferAccessFrequency::Dynamic, BufferAccessType::Draw };

const BufferUsage BufferUsage::stream_read{ BufferAccessFrequency::Stream, BufferAccessType::Read };
const BufferUsage BufferUsage::static_read{ BufferAccessFrequency::Static, BufferAccessType::Read };
const BufferUsage BufferUsage::dynamic_read{ BufferAccessFrequency::Dynamic, BufferAccessType::Read };

const BufferUsage BufferUsage::stream_copy{ BufferAccessFrequency::Stream, BufferAccessType::Copy };
const BufferUsage BufferUsage::static_copy{ BufferAccessFrequency::Static, BufferAccessType::Copy };
const BufferUsage BufferUsage::dynamic_copy{ BufferAccessFrequency::Dynamic, BufferAccessType::Copy };

// --------------------------- GlBuffer ---------------------------

Buffer::Buffer()
{
    create();
}

auto Buffer::create_static_with_size(u32 size_bytes) -> Buffer
{
    Buffer buffer;
    buffer.init_static_with_size(size_bytes);
    return buffer;
}

auto Buffer::create_static_with_data(const void* data, u32 data_size_bytes) -> Buffer
{
    Buffer buffer;
    buffer.init_static_with_data(data, data_size_bytes);
    return buffer;
}

auto Buffer::create_dynamic(BufferUsage usage) -> Buffer
{
    Buffer buffer;
    buffer.init_dynamic(usage);
    return buffer;
}

auto Buffer::create_dynamic_with_size(u32 size_bytes, BufferUsage usage) -> Buffer
{
    Buffer buffer;
    buffer.init_dynamic_with_size(size_bytes, usage);
    return buffer;
}

auto Buffer::create_dynamic_with_data(const void* data, u32 data_size_bytes, BufferUsage usage) -> Buffer
{
    Buffer buffer;
    buffer.init_dynamic_with_data(data, data_size_bytes, usage);
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

auto Buffer::init_static_with_size(u32 size_bytes) -> void
{
    init_static_with_data(nullptr, size_bytes);
}

auto Buffer::init_static_with_data(const void* data, u32 data_size_bytes) -> void
{
    ZTH_ASSERT(_state == BufferState::Uninitialized);
    ZTH_ASSERT(data_size_bytes != 0); // can't initialize a static buffer with size 0

    _size_bytes = data_size_bytes;
    glNamedBufferStorage(_id, _size_bytes, data, GL_DYNAMIC_STORAGE_BIT);

    _state = BufferState::InitializedStatic;
}

auto Buffer::init_dynamic(BufferUsage usage) -> void
{
    init_dynamic_with_data(nullptr, 0, usage);
}

auto Buffer::init_dynamic_with_size(u32 size_bytes, BufferUsage usage) -> void
{
    init_dynamic_with_data(nullptr, size_bytes, usage);
}

auto Buffer::init_dynamic_with_data(const void* data, u32 data_size_bytes, BufferUsage usage) -> void
{
    ZTH_ASSERT(_state != BufferState::InitializedStatic);

    _size_bytes = data_size_bytes;
    _usage = usage;
    glNamedBufferData(_id, _size_bytes, data, to_gl_enum(*_usage));

    _state = BufferState::InitializedDynamic;
}

auto Buffer::buffer_data(const void* data, u32 data_size_bytes, u32 offset) -> u32
{
    ZTH_ASSERT(_state != BufferState::Uninitialized);

    if (_state == BufferState::InitializedStatic)
    {
        ZTH_ASSERT(offset + data_size_bytes <= _size_bytes);
        glNamedBufferSubData(_id, offset, data_size_bytes, data);
    }
    else if (_state == BufferState::InitializedDynamic)
    {
        if (offset + data_size_bytes > _size_bytes)
            resize_to_at_least(offset + data_size_bytes);

        glNamedBufferSubData(_id, offset, data_size_bytes, data);
    }
    else
    {
        ZTH_ASSERT(false);
    }

    return data_size_bytes;
}

auto Buffer::create() -> void
{
    glCreateBuffers(1, &_id);
}

auto Buffer::destroy() -> void
{
    glDeleteBuffers(1, &_id);
}

auto Buffer::resize_to_at_least(u32 min_size_bytes) -> void
{
    ZTH_ASSERT(_state == BufferState::InitializedDynamic);

    if (min_size_bytes <= _size_bytes)
        return;

    auto old_size = _size_bytes;
    auto new_size = old_size * 2 + 1;

    // @cleanup: could maybe do this a cleaner way
    while (new_size < min_size_bytes)
        new_size *= 2;

    if (old_size != 0)
    {
        // need to copy the buffer's contents to newly allocated memory

        Buffer tmp_buffer = create_static_with_size(old_size);
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

auto VertexBuffer::create_static_with_size(u32 size) -> VertexBuffer
{
    VertexBuffer buffer;
    buffer.init_static_with_size(size);
    return buffer;
}

auto VertexBuffer::create_static_with_data(const void* data, u32 data_size_bytes) -> VertexBuffer
{
    VertexBuffer buffer;
    buffer.init_static_with_data(data, data_size_bytes);
    return buffer;
}

auto VertexBuffer::create_dynamic(BufferUsage usage) -> VertexBuffer
{
    VertexBuffer buffer;
    buffer.init_dynamic(usage);
    return buffer;
}

auto VertexBuffer::create_dynamic_with_size(u32 size, BufferUsage usage) -> VertexBuffer
{
    VertexBuffer buffer;
    buffer.init_dynamic_with_size(size, usage);
    return buffer;
}

auto VertexBuffer::create_dynamic_with_data(const void* data, u32 data_size_bytes, BufferUsage usage) -> VertexBuffer
{
    VertexBuffer buffer;
    buffer.init_dynamic_with_data(data, data_size_bytes, usage);
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

auto VertexBuffer::init_static_with_size(u32 size) -> void
{
    _buffer.init_static_with_size(size);
}

auto VertexBuffer::init_static_with_data(const void* data, u32 data_size_bytes) -> void
{
    _buffer.init_static_with_data(data, data_size_bytes);
}

auto VertexBuffer::init_dynamic(BufferUsage usage) -> void
{
    _buffer.init_dynamic(usage);
}

auto VertexBuffer::init_dynamic_with_size(u32 size, BufferUsage usage) -> void
{
    _buffer.init_dynamic_with_size(size, usage);
}

auto VertexBuffer::init_dynamic_with_data(const void* data, u32 data_size_bytes, BufferUsage usage) -> void
{
    _buffer.init_dynamic_with_data(data, data_size_bytes, usage);
}

auto VertexBuffer::buffer_data(const void* data, u32 data_size_bytes, u32 offset) -> u32
{
    return _buffer.buffer_data(data, data_size_bytes, offset);
}

auto VertexBuffer::bind() const -> void
{
    glBindBuffer(GL_ARRAY_BUFFER, native_handle());
}

auto VertexBuffer::unbind() -> void
{
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
}

auto VertexBuffer::set_stride(u32 stride) -> void
{
    _stride = stride;
}

// --------------------------- IndexBuffer ---------------------------

auto IndexBuffer::create_static_with_size(u32 size) -> IndexBuffer
{
    IndexBuffer buffer;
    buffer.init_static_with_size(size);
    return buffer;
}

auto IndexBuffer::create_static_with_data(const void* data, u32 data_size_bytes) -> IndexBuffer
{
    IndexBuffer buffer;
    buffer.init_static_with_data(data, data_size_bytes);
    return buffer;
}

auto IndexBuffer::create_dynamic(BufferUsage usage) -> IndexBuffer
{
    IndexBuffer buffer;
    buffer.init_dynamic(usage);
    return buffer;
}

auto IndexBuffer::create_dynamic_with_size(u32 size, BufferUsage usage) -> IndexBuffer
{
    IndexBuffer buffer;
    buffer.init_dynamic_with_size(size, usage);
    return buffer;
}

auto IndexBuffer::create_dynamic_with_data(const void* data, u32 data_size_bytes, BufferUsage usage) -> IndexBuffer
{
    IndexBuffer buffer;
    buffer.init_dynamic_with_data(data, data_size_bytes, usage);
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

auto IndexBuffer::init_static_with_size(u32 size) -> void
{
    _buffer.init_static_with_size(size);
}

auto IndexBuffer::init_static_with_data(const void* data, u32 data_size_bytes) -> void
{
    _buffer.init_static_with_data(data, data_size_bytes);
}

auto IndexBuffer::init_dynamic(BufferUsage usage) -> void
{
    _buffer.init_dynamic(usage);
}

auto IndexBuffer::init_dynamic_with_size(u32 size, BufferUsage usage) -> void
{
    _buffer.init_dynamic_with_size(size, usage);
}

auto IndexBuffer::init_dynamic_with_data(const void* data, u32 data_size_bytes, BufferUsage usage) -> void
{
    _buffer.init_dynamic_with_data(data, data_size_bytes, usage);
}

auto IndexBuffer::buffer_data(const void* data, u32 data_size_bytes, u32 offset) -> u32
{
    return _buffer.buffer_data(data, data_size_bytes, offset);
}

auto IndexBuffer::bind() const -> void
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, native_handle());
}

auto IndexBuffer::unbind() -> void
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);
}

auto IndexBuffer::set_index_type(GLenum index_type) -> void
{
    _index_type = index_type;
}

auto IndexBuffer::set_size(u32 size) -> void
{
    _size = size;
}

// --------------------------- InstanceBuffer ---------------------------

auto InstanceBuffer::create_static_with_size(u32 size) -> InstanceBuffer
{
    InstanceBuffer buffer;
    buffer.init_static_with_size(size);
    return buffer;
}

auto InstanceBuffer::create_static_with_data(const void* data, u32 data_size_bytes) -> InstanceBuffer
{
    InstanceBuffer buffer;
    buffer.init_static_with_data(data, data_size_bytes);
    return buffer;
}

auto InstanceBuffer::create_dynamic(BufferUsage usage) -> InstanceBuffer
{
    InstanceBuffer buffer;
    buffer.init_dynamic(usage);
    return buffer;
}

auto InstanceBuffer::create_dynamic_with_size(u32 size, BufferUsage usage) -> InstanceBuffer
{
    InstanceBuffer buffer;
    buffer.init_dynamic_with_size(size, usage);
    return buffer;
}

auto InstanceBuffer::create_dynamic_with_data(const void* data, u32 data_size_bytes,
                                              BufferUsage usage) -> InstanceBuffer
{
    InstanceBuffer buffer;
    buffer.init_dynamic_with_data(data, data_size_bytes, usage);
    return buffer;
}

// --------------------------- UniformBuffer ---------------------------

auto UniformBuffer::create_static_with_size(u32 size) -> UniformBuffer
{
    UniformBuffer buffer;
    buffer.init_static_with_size(size);
    return buffer;
}

auto UniformBuffer::create_static_with_size(u32 size, u32 binding_point) -> UniformBuffer
{
    UniformBuffer buffer;
    buffer.init_static_with_size(size, binding_point);
    return buffer;
}

auto UniformBuffer::create_static_with_data(const void* data, u32 data_size_bytes) -> UniformBuffer
{
    UniformBuffer buffer;
    buffer.init_static_with_data(data, data_size_bytes);
    return buffer;
}

auto UniformBuffer::create_static_with_data(const void* data, u32 data_size_bytes, u32 binding_point) -> UniformBuffer
{
    UniformBuffer buffer;
    buffer.init_static_with_data(data, data_size_bytes, binding_point);
    return buffer;
}

auto UniformBuffer::init_static_with_size(u32 size) -> void
{
    _buffer.init_static_with_size(size);
}

auto UniformBuffer::init_static_with_size(u32 size, u32 binding_point) -> void
{
    init_static_with_size(size);
    bind(binding_point);
}

auto UniformBuffer::init_static_with_data(const void* data, u32 data_size_bytes) -> void
{
    _buffer.init_static_with_data(data, data_size_bytes);
}

auto UniformBuffer::init_static_with_data(const void* data, u32 data_size_bytes, u32 binding_point) -> void
{
    init_static_with_data(data, data_size_bytes);
    bind(binding_point);
}

auto UniformBuffer::buffer_data(const void* data, u32 data_size_bytes, u32 offset) -> u32
{
    return _buffer.buffer_data(data, data_size_bytes, offset);
}

auto UniformBuffer::bind() const -> void
{
    glBindBuffer(GL_UNIFORM_BUFFER, native_handle());
}

auto UniformBuffer::bind(u32 binding_point) const -> void
{
    glBindBufferBase(GL_UNIFORM_BUFFER, binding_point, native_handle());
}

auto UniformBuffer::unbind() -> void
{
    glBindBuffer(GL_UNIFORM_BUFFER, GL_NONE);
}

// --------------------------- ShaderStorageBuffer ---------------------------

auto ShaderStorageBuffer::create_static_with_size(u32 size) -> ShaderStorageBuffer
{
    ShaderStorageBuffer buffer;
    buffer.init_static_with_size(size);
    return buffer;
}

auto ShaderStorageBuffer::create_static_with_size(u32 size, u32 binding_point) -> ShaderStorageBuffer
{
    ShaderStorageBuffer buffer;
    buffer.init_static_with_size(size, binding_point);
    return buffer;
}

auto ShaderStorageBuffer::create_static_with_data(const void* data, u32 data_size_bytes) -> ShaderStorageBuffer
{
    ShaderStorageBuffer buffer;
    buffer.init_static_with_data(data, data_size_bytes);
    return buffer;
}

auto ShaderStorageBuffer::create_static_with_data(const void* data, u32 data_size_bytes,
                                                  u32 binding_point) -> ShaderStorageBuffer
{
    ShaderStorageBuffer buffer;
    buffer.init_static_with_data(data, data_size_bytes, binding_point);
    return buffer;
}

auto ShaderStorageBuffer::create_dynamic(BufferUsage usage) -> ShaderStorageBuffer
{
    ShaderStorageBuffer buffer;
    buffer.init_dynamic(usage);
    return buffer;
}

auto ShaderStorageBuffer::create_dynamic(u32 binding_point, BufferUsage usage) -> ShaderStorageBuffer
{
    ShaderStorageBuffer buffer;
    buffer.init_dynamic(binding_point, usage);
    return buffer;
}

auto ShaderStorageBuffer::create_dynamic_with_size(u32 size, BufferUsage usage) -> ShaderStorageBuffer
{
    ShaderStorageBuffer buffer;
    buffer.init_dynamic_with_size(size, usage);
    return buffer;
}

auto ShaderStorageBuffer::create_dynamic_with_size(u32 size, u32 binding_point,
                                                   BufferUsage usage) -> ShaderStorageBuffer
{
    ShaderStorageBuffer buffer;
    buffer.init_dynamic_with_size(size, binding_point, usage);
    return buffer;
}

auto ShaderStorageBuffer::create_dynamic_with_data(const void* data, u32 data_size_bytes,
                                                   BufferUsage usage) -> ShaderStorageBuffer
{
    ShaderStorageBuffer buffer;
    buffer.init_dynamic_with_data(data, data_size_bytes, usage);
    return buffer;
}

auto ShaderStorageBuffer::create_dynamic_with_data(const void* data, u32 data_size_bytes, u32 binding_point,
                                                   BufferUsage usage) -> ShaderStorageBuffer
{
    ShaderStorageBuffer buffer;
    buffer.init_dynamic_with_data(data, data_size_bytes, binding_point, usage);
    return buffer;
}

auto ShaderStorageBuffer::init_static_with_size(u32 size) -> void
{
    _buffer.init_static_with_size(size);
}

auto ShaderStorageBuffer::init_static_with_size(u32 size, u32 binding_point) -> void
{
    init_static_with_size(size);
    bind(binding_point);
}

auto ShaderStorageBuffer::init_static_with_data(const void* data, u32 data_size_bytes) -> void
{
    _buffer.init_static_with_data(data, data_size_bytes);
}

auto ShaderStorageBuffer::init_static_with_data(const void* data, u32 data_size_bytes, u32 binding_point) -> void
{
    init_static_with_data(data, data_size_bytes);
    bind(binding_point);
}

auto ShaderStorageBuffer::init_dynamic(BufferUsage usage) -> void
{
    _buffer.init_dynamic(usage);
}

auto ShaderStorageBuffer::init_dynamic(u32 binding_point, BufferUsage usage) -> void
{
    init_dynamic(usage);
    bind(binding_point);
}

auto ShaderStorageBuffer::init_dynamic_with_size(u32 size, BufferUsage usage) -> void
{
    _buffer.init_dynamic_with_size(size, usage);
}

auto ShaderStorageBuffer::init_dynamic_with_size(u32 size, u32 binding_point, BufferUsage usage) -> void
{
    init_dynamic_with_size(size, usage);
    bind(binding_point);
}

auto ShaderStorageBuffer::init_dynamic_with_data(const void* data, u32 data_size_bytes, BufferUsage usage) -> void
{
    _buffer.init_dynamic_with_data(data, data_size_bytes, usage);
}

auto ShaderStorageBuffer::init_dynamic_with_data(const void* data, u32 data_size_bytes, u32 binding_point,
                                                 BufferUsage usage) -> void
{
    init_dynamic_with_data(data, data_size_bytes, usage);
    bind(binding_point);
}

auto ShaderStorageBuffer::buffer_data(const void* data, u32 data_size_bytes, u32 offset) -> u32
{
    return _buffer.buffer_data(data, data_size_bytes, offset);
}

auto ShaderStorageBuffer::bind() const -> void
{
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, native_handle());
}

auto ShaderStorageBuffer::bind(u32 binding_point) const -> void
{
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding_point, native_handle());
}

auto ShaderStorageBuffer::unbind() -> void
{
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, GL_NONE);
}

auto to_gl_enum(BufferUsage buffer_usage) -> GLenum
{
    GLenum res = GL_STREAM_DRAW;
    res += std::to_underlying(buffer_usage.access_frequency);
    res += std::to_underlying(buffer_usage.access_type);
    return res;
}

} // namespace zth::gl
