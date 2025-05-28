#include "zenith/gl/buffer.hpp"

#include "zenith/core/assert.hpp"

namespace zth::gl {

constinit const BufferUsage BufferUsage::stream_draw{ BufferAccessFrequency::Stream, BufferAccessType::Draw };
constinit const BufferUsage BufferUsage::static_draw{ BufferAccessFrequency::Static, BufferAccessType::Draw };
constinit const BufferUsage BufferUsage::dynamic_draw{ BufferAccessFrequency::Dynamic, BufferAccessType::Draw };

constinit const BufferUsage BufferUsage::stream_read{ BufferAccessFrequency::Stream, BufferAccessType::Read };
constinit const BufferUsage BufferUsage::static_read{ BufferAccessFrequency::Static, BufferAccessType::Read };
constinit const BufferUsage BufferUsage::dynamic_read{ BufferAccessFrequency::Dynamic, BufferAccessType::Read };

constinit const BufferUsage BufferUsage::stream_copy{ BufferAccessFrequency::Stream, BufferAccessType::Copy };
constinit const BufferUsage BufferUsage::static_copy{ BufferAccessFrequency::Static, BufferAccessType::Copy };
constinit const BufferUsage BufferUsage::dynamic_copy{ BufferAccessFrequency::Dynamic, BufferAccessType::Copy };

// --------------------------- Buffer ---------------------------

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

auto Buffer::create_static_with_data(std::span<const byte> data) -> Buffer
{
    Buffer buffer;
    buffer.init_static_with_data(data);
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

auto Buffer::create_dynamic_with_data(std::span<const byte> data, BufferUsage usage) -> Buffer
{
    Buffer buffer;
    buffer.init_dynamic_with_data(data, usage);
    return buffer;
}

Buffer::Buffer(const Buffer& other) : Buffer()
{
    copy_initialize(other);
}

auto Buffer::operator=(const Buffer& other) -> Buffer&
{
    if (this == &other)
        return *this;

    free();
    copy_initialize(other);

    return *this;
}

Buffer::Buffer(Buffer&& other) noexcept
    : _id{ std::exchange(other._id, GL_NONE) }, _size_bytes{ std::exchange(other._size_bytes, 0) },
      _capacity_bytes{ std::exchange(other._capacity_bytes, 0) },
      _state{ std::exchange(other._state, BufferState::Uninitialized) }, _usage{ std::exchange(other._usage, nil) }
{}

auto Buffer::operator=(Buffer&& other) noexcept -> Buffer&
{
    free();

    _id = std::exchange(other._id, GL_NONE);
    _size_bytes = std::exchange(other._size_bytes, 0);
    _capacity_bytes = std::exchange(other._capacity_bytes, 0);
    _state = std::exchange(other._state, BufferState::Uninitialized);
    _usage = std::exchange(other._usage, nil);

    return *this;
}

Buffer::~Buffer()
{
    free();
}

auto Buffer::init_static_with_size(u32 size_bytes) -> void
{
    ZTH_ASSERT(_state == BufferState::Uninitialized);
    ZTH_ASSERT(size_bytes != 0); // Can't initialize a static buffer with size 0.

    _size_bytes = size_bytes;
    _capacity_bytes = size_bytes;
    glNamedBufferStorage(_id, _size_bytes, nullptr, GL_DYNAMIC_STORAGE_BIT);

    _state = BufferState::InitializedStatic;
}

auto Buffer::init_static_with_data(std::span<const byte> data) -> void
{
    ZTH_ASSERT(_state == BufferState::Uninitialized);
    ZTH_ASSERT(data.size_bytes() != 0); // Can't initialize a static buffer with size 0.

    _size_bytes = static_cast<u32>(data.size_bytes());
    _capacity_bytes = static_cast<u32>(data.size_bytes());
    glNamedBufferStorage(_id, _size_bytes, data.data(), GL_DYNAMIC_STORAGE_BIT);

    _state = BufferState::InitializedStatic;
}

auto Buffer::init_dynamic(BufferUsage usage) -> void
{
    init_dynamic_with_size(0, usage);
}

auto Buffer::init_dynamic_with_size(u32 size_bytes, BufferUsage usage) -> void
{
    ZTH_ASSERT(_state != BufferState::InitializedStatic); // Dynamic buffers can be reinitialized.

    _size_bytes = size_bytes;
    _capacity_bytes = size_bytes;
    _usage = usage;
    glNamedBufferData(_id, _size_bytes, nullptr, to_gl_enum(*_usage));

    _state = BufferState::InitializedDynamic;
}

auto Buffer::init_dynamic_with_data(std::span<const byte> data, BufferUsage usage) -> void
{
    ZTH_ASSERT(_state != BufferState::InitializedStatic); // Dynamic buffers can be reinitialized.

    _size_bytes = static_cast<u32>(data.size_bytes());
    _capacity_bytes = static_cast<u32>(data.size_bytes());
    _usage = usage;
    glNamedBufferData(_id, _size_bytes, data.data(), to_gl_enum(*_usage));

    _state = BufferState::InitializedDynamic;
}

auto Buffer::buffer_data(std::span<const byte> data, u32 offset) -> u32
{
    ZTH_ASSERT(_state != BufferState::Uninitialized);

    if (_state == BufferState::InitializedStatic)
        return buffer_data_static(data, offset);
    else if (_state == BufferState::InitializedDynamic)
        return buffer_data_dynamic(data, offset);

    ZTH_ASSERT(false);
    return 0;
}

auto Buffer::append_data(std::span<const byte> data) -> u32
{
    ZTH_ASSERT(_state == BufferState::InitializedDynamic);
    return buffer_data(data, _size_bytes);
}

auto Buffer::resize(u32 size_bytes) -> void
{
    ZTH_ASSERT(_state == BufferState::InitializedDynamic);
    reserve(size_bytes);
    _size_bytes = size_bytes;
}

auto Buffer::resize_to_at_least(u32 min_size_bytes) -> void
{
    ZTH_ASSERT(_state == BufferState::InitializedDynamic);
    resize(std::max(_size_bytes, min_size_bytes));
}

auto Buffer::reserve(u32 min_size_bytes) -> void
{
    ZTH_ASSERT(_state == BufferState::InitializedDynamic);
    reallocate_at_least(min_size_bytes);
}

auto Buffer::shrink_to_fit() -> void
{
    ZTH_ASSERT(_state == BufferState::InitializedDynamic);
    ZTH_ASSERT(_size_bytes <= _capacity_bytes);

    if (_size_bytes == _capacity_bytes)
        return;

    reallocate_exactly(_size_bytes);
}

auto Buffer::clear() -> void
{
    ZTH_ASSERT(_state == BufferState::InitializedDynamic);
    resize(0);
}

auto Buffer::free() noexcept -> void
{
    destroy();

    _id = GL_NONE;
    _size_bytes = 0;
    _capacity_bytes = 0;
    _state = BufferState::Uninitialized;
    _usage = nil;
}

auto Buffer::create() noexcept -> void
{
    glCreateBuffers(1, &_id);
}

auto Buffer::destroy() noexcept -> void
{
    glDeleteBuffers(1, &_id);
}

auto Buffer::copy_initialize(const Buffer& other) -> void
{
    switch (other.state())
    {
        using enum BufferState;
    case Uninitialized:
    {
        // Don't have to do anything.
    }
    break;
    case InitializedStatic:
    {
        init_static_with_size(other._size_bytes);
        copy_buffer_data(*this, other, other._size_bytes);
    }
    break;
    case InitializedDynamic:
    {
        ZTH_ASSERT(other._usage.has_value());
        init_dynamic_with_size(other._size_bytes, *other._usage);
        copy_buffer_data(*this, other, other._size_bytes);
    }
    break;
    }
}

auto Buffer::buffer_data_static(std::span<const byte> data, u32 offset) -> u32
{
    ZTH_ASSERT(_state == BufferState::InitializedStatic);
    ZTH_ASSERT(offset + data.size_bytes() <= _size_bytes);
    glNamedBufferSubData(_id, offset, static_cast<GLsizeiptr>(data.size_bytes()), data.data());
    return static_cast<u32>(data.size_bytes());
}

auto Buffer::buffer_data_dynamic(std::span<const byte> data, u32 offset) -> u32
{
    ZTH_ASSERT(_state == BufferState::InitializedDynamic);
    resize_to_at_least(offset + static_cast<u32>(data.size_bytes()));
    glNamedBufferSubData(_id, offset, static_cast<GLsizeiptr>(data.size_bytes()), data.data());
    return static_cast<u32>(data.size_bytes());
}

auto Buffer::reallocate_exactly(u32 new_capacity_bytes) -> void
{
    ZTH_ASSERT(_state == BufferState::InitializedDynamic);

    if (_size_bytes != 0) // Can't create a static temporary buffer with size 0.
    {
        // We need to copy the buffer's contents to newly allocated memory. We can't just create a new buffer and free
        // the old one because the id of this buffer needs to remain unchanged.

        // @speed: We could probably some kind of globally available temporary buffer here instead of always creating a
        // new one.

        auto bytes_to_copy = std::min(_size_bytes, new_capacity_bytes);
        Buffer tmp_buffer = create_static_with_size(bytes_to_copy);
        copy_buffer_data(tmp_buffer, *this, bytes_to_copy);

        ZTH_ASSERT(_usage.has_value());
        init_dynamic_with_size(new_capacity_bytes, *_usage);
        copy_buffer_data(*this, tmp_buffer, bytes_to_copy);
    }
    else
    {
        ZTH_ASSERT(_usage.has_value());
        init_dynamic_with_size(new_capacity_bytes, *_usage);
    }

    _capacity_bytes = new_capacity_bytes;
}

auto Buffer::reallocate_at_least(u32 min_capacity_bytes) -> void
{
    ZTH_ASSERT(_state == BufferState::InitializedDynamic);

    if (_capacity_bytes >= min_capacity_bytes)
        return;

    auto new_capacity = std::max(calculate_growth(_capacity_bytes), min_capacity_bytes);
    reallocate_exactly(new_capacity);
}

auto Buffer::calculate_growth(u32 old_size_bytes) -> u32
{
    return old_size_bytes + old_size_bytes / 2;
}

// --------------------------- VertexBuffer ---------------------------

auto VertexBuffer::create_static_with_size(u32 size_bytes, const VertexLayout& layout) -> VertexBuffer
{
    VertexBuffer buffer;
    buffer.init_static_with_size(size_bytes, layout);
    return buffer;
}

auto VertexBuffer::create_static_with_data(std::span<const byte> data, const VertexLayout& layout) -> VertexBuffer
{
    VertexBuffer buffer;
    buffer.init_static_with_data(data, layout);
    return buffer;
}

auto VertexBuffer::create_dynamic(const VertexLayout& layout, BufferUsage usage) -> VertexBuffer
{
    VertexBuffer buffer;
    buffer.init_dynamic(layout, usage);
    return buffer;
}

auto VertexBuffer::create_dynamic_with_size(u32 size_bytes, const VertexLayout& layout, BufferUsage usage)
    -> VertexBuffer
{
    VertexBuffer buffer;
    buffer.init_dynamic_with_size(size_bytes, layout, usage);
    return buffer;
}

auto VertexBuffer::create_dynamic_with_data(std::span<const byte> data, const VertexLayout& layout, BufferUsage usage)
    -> VertexBuffer
{
    VertexBuffer buffer;
    buffer.init_dynamic_with_data(data, layout, usage);
    return buffer;
}

auto VertexBuffer::init_static_with_size(u32 size_bytes, const VertexLayout& layout) -> void
{
    _buffer.init_static_with_size(size_bytes);
    _layout = layout;
}

auto VertexBuffer::init_static_with_data(std::span<const byte> data, const VertexLayout& layout) -> void
{
    _buffer.init_static_with_data(data);
    _layout = layout;
}

auto VertexBuffer::init_dynamic(const VertexLayout& layout, BufferUsage usage) -> void
{
    _buffer.init_dynamic(usage);
    _layout = layout;
}

auto VertexBuffer::init_dynamic_with_size(u32 size_bytes, const VertexLayout& layout, BufferUsage usage) -> void
{
    _buffer.init_dynamic_with_size(size_bytes, usage);
    _layout = layout;
}

auto VertexBuffer::init_dynamic_with_data(std::span<const byte> data, const VertexLayout& layout, BufferUsage usage)
    -> void
{
    _buffer.init_dynamic_with_data(data, usage);
    _layout = layout;
}

auto VertexBuffer::buffer_data(std::span<const byte> data, u32 offset) -> u32
{
    return _buffer.buffer_data(data, offset);
}

auto VertexBuffer::append_data(std::span<const byte> data) -> u32
{
    return _buffer.append_data(data);
}

auto VertexBuffer::free() noexcept -> void
{
    _buffer.free();
    _layout.clear();
}

auto VertexBuffer::bind() const -> void
{
    glBindBuffer(GL_ARRAY_BUFFER, native_handle());
}

auto VertexBuffer::unbind() -> void
{
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
}

auto VertexBuffer::set_layout(const VertexLayout& layout) -> void
{
    _layout = layout;
}

auto VertexBuffer::set_stride(u32 stride_bytes) -> void
{
    _layout.set_stride(stride_bytes);
}

auto VertexBuffer::count() const -> u32
{
    return size_bytes() / stride();
}

// --------------------------- IndexBuffer ---------------------------

auto IndexBuffer::create_static_with_size(u32 size_bytes, DataType type) -> IndexBuffer
{
    IndexBuffer buffer;
    buffer.init_static_with_size(size_bytes, type);
    return buffer;
}

auto IndexBuffer::create_static_with_data(std::span<const byte> data, DataType type) -> IndexBuffer
{
    IndexBuffer buffer;
    buffer.init_static_with_data(data, type);
    return buffer;
}

auto IndexBuffer::create_dynamic(DataType type, BufferUsage usage) -> IndexBuffer
{
    IndexBuffer buffer;
    buffer.init_dynamic(type, usage);
    return buffer;
}

auto IndexBuffer::create_dynamic_with_size(u32 size_bytes, DataType type, BufferUsage usage) -> IndexBuffer
{
    IndexBuffer buffer;
    buffer.init_dynamic_with_size(size_bytes, type, usage);
    return buffer;
}

auto IndexBuffer::create_dynamic_with_data(std::span<const byte> data, DataType type, BufferUsage usage) -> IndexBuffer
{
    IndexBuffer buffer;
    buffer.init_dynamic_with_data(data, type, usage);
    return buffer;
}

IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
    : _buffer{ std::move(other._buffer) },
      _indexing_data_type{ std::exchange(other._indexing_data_type, DataType::None) }
{}

auto IndexBuffer::operator=(IndexBuffer&& other) noexcept -> IndexBuffer&
{
    _buffer = std::move(other._buffer);
    _indexing_data_type = std::exchange(other._indexing_data_type, DataType::None);
    return *this;
}

auto IndexBuffer::init_static_with_size(u32 size_bytes, DataType type) -> void
{
    _buffer.init_static_with_size(size_bytes);
    set_indexing_data_type(type);
}

auto IndexBuffer::init_static_with_data(std::span<const byte> data, DataType type) -> void
{
    _buffer.init_static_with_data(data);
    set_indexing_data_type(type);
}

auto IndexBuffer::init_dynamic(DataType type, BufferUsage usage) -> void
{
    _buffer.init_dynamic(usage);
    set_indexing_data_type(type);
}

auto IndexBuffer::init_dynamic_with_size(u32 size_bytes, DataType type, BufferUsage usage) -> void
{
    _buffer.init_dynamic_with_size(size_bytes, usage);
    set_indexing_data_type(type);
}

auto IndexBuffer::init_dynamic_with_data(std::span<const byte> data, DataType type, BufferUsage usage) -> void
{
    _buffer.init_dynamic_with_data(data, usage);
    set_indexing_data_type(type);
}

auto IndexBuffer::buffer_data(std::span<const byte> data, u32 offset) -> u32
{
    return _buffer.buffer_data(data, offset);
}

auto IndexBuffer::append_data(std::span<const byte> data) -> u32
{
    return _buffer.append_data(data);
}

auto IndexBuffer::free() noexcept -> void
{
    _buffer.free();
    _indexing_data_type = DataType::None;
}

auto IndexBuffer::bind() const -> void
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, native_handle());
}

auto IndexBuffer::unbind() -> void
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);
}

auto IndexBuffer::set_indexing_data_type(DataType type) -> void
{
    ZTH_ASSERT(is_an_indexing_data_type(type));
    _indexing_data_type = type;
}

auto IndexBuffer::count() const -> u32
{
    return size_bytes() / static_cast<u32>(size_of_data_type(_indexing_data_type));
}

// --------------------------- InstanceBuffer ---------------------------

auto InstanceBuffer::create_static_with_size(u32 size_bytes, const VertexLayout& layout) -> InstanceBuffer
{
    InstanceBuffer buffer;
    buffer.init_static_with_size(size_bytes, layout);
    return buffer;
}

auto InstanceBuffer::create_static_with_data(std::span<const byte> data, const VertexLayout& layout) -> InstanceBuffer
{
    InstanceBuffer buffer;
    buffer.init_static_with_data(data, layout);
    return buffer;
}

auto InstanceBuffer::create_dynamic(const VertexLayout& layout, BufferUsage usage) -> InstanceBuffer
{
    InstanceBuffer buffer;
    buffer.init_dynamic(layout, usage);
    return buffer;
}

auto InstanceBuffer::create_dynamic_with_size(u32 size_bytes, const VertexLayout& layout, BufferUsage usage)
    -> InstanceBuffer
{
    InstanceBuffer buffer;
    buffer.init_dynamic_with_size(size_bytes, layout, usage);
    return buffer;
}

auto InstanceBuffer::create_dynamic_with_data(std::span<const byte> data, const VertexLayout& layout, BufferUsage usage)
    -> InstanceBuffer
{
    InstanceBuffer buffer;
    buffer.init_dynamic_with_data(data, layout, usage);
    return buffer;
}

// --------------------------- UniformBuffer ---------------------------

auto UniformBuffer::create_static_with_size(u32 size_bytes) -> UniformBuffer
{
    UniformBuffer buffer;
    buffer.init_static_with_size(size_bytes);
    return buffer;
}

auto UniformBuffer::create_static_with_size(u32 size_bytes, u32 binding_point) -> UniformBuffer
{
    UniformBuffer buffer;
    buffer.init_static_with_size(size_bytes, binding_point);
    return buffer;
}

auto UniformBuffer::create_static_with_data(std::span<const byte> data) -> UniformBuffer
{
    UniformBuffer buffer;
    buffer.init_static_with_data(data);
    return buffer;
}

auto UniformBuffer::create_static_with_data(std::span<const byte> data, u32 binding_point) -> UniformBuffer
{
    UniformBuffer buffer;
    buffer.init_static_with_data(data, binding_point);
    return buffer;
}

auto UniformBuffer::init_static_with_size(u32 size_bytes) -> void
{
    _buffer.init_static_with_size(size_bytes);
}

auto UniformBuffer::init_static_with_size(u32 size_bytes, u32 binding_point) -> void
{
    init_static_with_size(size_bytes);
    bind(binding_point);
}

auto UniformBuffer::init_static_with_data(std::span<const byte> data) -> void
{
    _buffer.init_static_with_data(data);
}

auto UniformBuffer::init_static_with_data(std::span<const byte> data, u32 binding_point) -> void
{
    init_static_with_data(data);
    bind(binding_point);
}

auto UniformBuffer::buffer_data(std::span<const byte> data, u32 offset) -> u32
{
    return _buffer.buffer_data(data, offset);
}

auto UniformBuffer::free() noexcept -> void
{
    _buffer.free();
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

auto ShaderStorageBuffer::create_static_with_size(u32 size_bytes) -> ShaderStorageBuffer
{
    ShaderStorageBuffer buffer;
    buffer.init_static_with_size(size_bytes);
    return buffer;
}

auto ShaderStorageBuffer::create_static_with_size(u32 size_bytes, u32 binding_point) -> ShaderStorageBuffer
{
    ShaderStorageBuffer buffer;
    buffer.init_static_with_size(size_bytes, binding_point);
    return buffer;
}

auto ShaderStorageBuffer::create_static_with_data(std::span<const byte> data) -> ShaderStorageBuffer
{
    ShaderStorageBuffer buffer;
    buffer.init_static_with_data(data);
    return buffer;
}

auto ShaderStorageBuffer::create_static_with_data(std::span<const byte> data, u32 binding_point) -> ShaderStorageBuffer
{
    ShaderStorageBuffer buffer;
    buffer.init_static_with_data(data, binding_point);
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

auto ShaderStorageBuffer::create_dynamic_with_size(u32 size_bytes, BufferUsage usage) -> ShaderStorageBuffer
{
    ShaderStorageBuffer buffer;
    buffer.init_dynamic_with_size(size_bytes, usage);
    return buffer;
}

auto ShaderStorageBuffer::create_dynamic_with_size(u32 size_bytes, u32 binding_point, BufferUsage usage)
    -> ShaderStorageBuffer
{
    ShaderStorageBuffer buffer;
    buffer.init_dynamic_with_size(size_bytes, binding_point, usage);
    return buffer;
}

auto ShaderStorageBuffer::create_dynamic_with_data(std::span<const byte> data, BufferUsage usage) -> ShaderStorageBuffer
{
    ShaderStorageBuffer buffer;
    buffer.init_dynamic_with_data(data, usage);
    return buffer;
}

auto ShaderStorageBuffer::create_dynamic_with_data(std::span<const byte> data, u32 binding_point, BufferUsage usage)
    -> ShaderStorageBuffer
{
    ShaderStorageBuffer buffer;
    buffer.init_dynamic_with_data(data, binding_point, usage);
    return buffer;
}

auto ShaderStorageBuffer::init_static_with_size(u32 size_bytes) -> void
{
    _buffer.init_static_with_size(size_bytes);
}

auto ShaderStorageBuffer::init_static_with_size(u32 size_bytes, u32 binding_point) -> void
{
    init_static_with_size(size_bytes);
    bind(binding_point);
}

auto ShaderStorageBuffer::init_static_with_data(std::span<const byte> data) -> void
{
    _buffer.init_static_with_data(data);
}

auto ShaderStorageBuffer::init_static_with_data(std::span<const byte> data, u32 binding_point) -> void
{
    init_static_with_data(data);
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

auto ShaderStorageBuffer::init_dynamic_with_size(u32 size_bytes, BufferUsage usage) -> void
{
    _buffer.init_dynamic_with_size(size_bytes, usage);
}

auto ShaderStorageBuffer::init_dynamic_with_size(u32 size_bytes, u32 binding_point, BufferUsage usage) -> void
{
    init_dynamic_with_size(size_bytes, usage);
    bind(binding_point);
}

auto ShaderStorageBuffer::init_dynamic_with_data(std::span<const byte> data, BufferUsage usage) -> void
{
    _buffer.init_dynamic_with_data(data, usage);
}

auto ShaderStorageBuffer::init_dynamic_with_data(std::span<const byte> data, u32 binding_point, BufferUsage usage)
    -> void
{
    init_dynamic_with_data(data, usage);
    bind(binding_point);
}

auto ShaderStorageBuffer::buffer_data(std::span<const byte> data, u32 offset) -> u32
{
    return _buffer.buffer_data(data, offset);
}

auto ShaderStorageBuffer::free() noexcept -> void
{
    _buffer.free();
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
