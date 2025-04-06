#include "zenith/memory/buffer.hpp"

#include "zenith/core/assert.hpp"
#include "zenith/memory/alloc.hpp"

namespace zth::memory {

Buffer::Buffer(size_type size_bytes) noexcept
{
    allocate(size_bytes);
}

Buffer::Buffer(const void* data, size_type data_size_bytes) noexcept : Buffer(data_size_bytes)
{
    buffer_data(data, data_size_bytes);
}

auto Buffer::with_size(size_type size_bytes) noexcept -> Buffer
{
    return Buffer{ size_bytes };
}

auto Buffer::with_data(const void* data, size_type data_size_bytes) noexcept -> Buffer
{
    return Buffer{ data, data_size_bytes };
}

Buffer::Buffer(const Buffer& other) noexcept : Buffer(other._size_bytes)
{
    std::memcpy(_data, other._data, _size_bytes);
}

auto Buffer::operator=(const Buffer& other) noexcept -> Buffer&
{
    if (this == &other)
        return *this;

    if (_size_bytes != other._size_bytes)
    {
        free();
        allocate(other._size_bytes);
    }

    std::memcpy(_data, other._data, _size_bytes);
    return *this;
}

Buffer::Buffer(Buffer&& other) noexcept
    : _size_bytes(std::exchange(other._size_bytes, 0)), _data(std::exchange(other._data, nullptr))
{}

auto Buffer::operator=(Buffer&& other) noexcept -> Buffer&
{
    free();

    _size_bytes = std::exchange(other._size_bytes, 0);
    _data = std::exchange(other._data, nullptr);

    return *this;
}

Buffer::~Buffer()
{
    free();
}

auto Buffer::cbegin() const noexcept -> const_iterator
{
    return begin();
}

auto Buffer::cend() const noexcept -> const_iterator
{
    return end();
}

auto Buffer::resize(size_type size_bytes) noexcept -> void
{
    if (_size_bytes == size_bytes)
        return;

    reallocate(size_bytes);
}

auto Buffer::resize_to_at_least(size_type min_size_bytes) noexcept -> void
{
    resize(std::max(_size_bytes, min_size_bytes));
}

auto Buffer::free() noexcept -> void
{
    _size_bytes = 0;
    memory::deallocate(_data);
}

auto Buffer::buffer_data(const void* data, size_type data_size_bytes, size_type offset) noexcept -> size_type
{
    ZTH_ASSERT(offset + data_size_bytes <= _size_bytes);
    std::memcpy(_data + offset, data, data_size_bytes);
    return data_size_bytes;
}

auto Buffer::allocate(size_type size_bytes) noexcept -> void
{
    _data = static_cast<byte*>(memory::allocate(size_bytes));
    _size_bytes = size_bytes;
}

auto Buffer::reallocate(size_type size_bytes) noexcept -> void
{
    memory::reallocate(_data, size_bytes, _size_bytes);
    _size_bytes = size_bytes;
}

DynamicBuffer::DynamicBuffer(size_type size_bytes) noexcept
{
    resize(size_bytes);
}

DynamicBuffer::DynamicBuffer(const void* data, size_type data_size_bytes) noexcept : DynamicBuffer(data_size_bytes)
{
    buffer_data(data, data_size_bytes);
}

auto DynamicBuffer::with_size(size_type size_bytes) noexcept -> DynamicBuffer
{
    return DynamicBuffer{ size_bytes };
}

auto DynamicBuffer::with_data(const void* data, size_type data_size_bytes) noexcept -> DynamicBuffer
{
    return DynamicBuffer{ data, data_size_bytes };
}

DynamicBuffer::DynamicBuffer(const DynamicBuffer& other) noexcept : DynamicBuffer(other._size_bytes)
{
    std::memcpy(_data, other._data, _size_bytes);
}

auto DynamicBuffer::operator=(const DynamicBuffer& other) noexcept -> DynamicBuffer&
{
    if (this == &other)
        return *this;

    if (_capacity_bytes < other._size_bytes)
    {
        free();
        allocate(other._size_bytes);
    }

    _size_bytes = other._size_bytes;
    std::memcpy(_data, other._data, _size_bytes);

    return *this;
}

DynamicBuffer::DynamicBuffer(DynamicBuffer&& other) noexcept
    : _size_bytes(std::exchange(other._size_bytes, 0)), _capacity_bytes(std::exchange(other._capacity_bytes, 0)),
      _data(std::exchange(other._data, nullptr))
{}

auto DynamicBuffer::operator=(DynamicBuffer&& other) noexcept -> DynamicBuffer&
{
    free();

    _size_bytes = std::exchange(other._size_bytes, 0);
    _capacity_bytes = std::exchange(other._capacity_bytes, 0);
    _data = std::exchange(other._data, nullptr);

    return *this;
}

DynamicBuffer::~DynamicBuffer()
{
    free();
}

auto DynamicBuffer::cbegin() const noexcept -> const_iterator
{
    return begin();
}

auto DynamicBuffer::cend() const noexcept -> const_iterator
{
    return end();
}

auto DynamicBuffer::resize(size_type size_bytes) noexcept -> void
{
    reserve(size_bytes);
    _size_bytes = size_bytes;
}

auto DynamicBuffer::resize_to_at_least(size_type min_size_bytes) noexcept -> void
{
    resize(std::max(_size_bytes, min_size_bytes));
}

auto DynamicBuffer::reserve(size_type min_capacity_bytes) noexcept -> void
{
    reallocate_at_least(min_capacity_bytes);
}

auto DynamicBuffer::shrink_to_fit() noexcept -> void
{
    ZTH_ASSERT(_size_bytes <= _capacity_bytes);

    if (_size_bytes == _capacity_bytes)
        return;

    reallocate_exactly(_size_bytes);
}

auto DynamicBuffer::free() noexcept -> void
{
    _size_bytes = 0;
    _capacity_bytes = 0;
    memory::deallocate(_data);
}

auto DynamicBuffer::buffer_data(const void* data, size_type data_size_bytes, size_type offset) noexcept -> size_type
{
    resize_to_at_least(offset + data_size_bytes);
    std::memcpy(_data + offset, data, data_size_bytes);
    return data_size_bytes;
}

auto DynamicBuffer::clear() noexcept -> void
{
    _size_bytes = 0;
}

auto DynamicBuffer::allocate(size_type capacity_bytes) noexcept -> void
{
    _data = static_cast<byte*>(memory::allocate(capacity_bytes));
    _capacity_bytes = capacity_bytes;
}

auto DynamicBuffer::reallocate_exactly(size_type new_capacity_bytes) noexcept -> void
{
    memory::reallocate(_data, new_capacity_bytes, _size_bytes);
    _capacity_bytes = new_capacity_bytes;
}

auto DynamicBuffer::reallocate_at_least(size_type min_capacity_bytes) noexcept -> void
{
    if (_capacity_bytes >= min_capacity_bytes)
        return;

    auto new_capacity = std::max(calculate_growth(_capacity_bytes), min_capacity_bytes);
    reallocate_exactly(new_capacity);
}

auto DynamicBuffer::calculate_growth(size_type old_size_bytes) noexcept -> size_type
{
    return old_size_bytes * 2;
}

} // namespace zth::memory
