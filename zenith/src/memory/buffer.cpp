#include "zenith/memory/buffer.hpp"

#include "zenith/core/assert.hpp"
#include "zenith/memory/alloc.hpp"

namespace zth {

Buffer::Buffer(usize size)
{
    allocate(size);
}

Buffer::Buffer(const void* data, usize data_size_bytes) : Buffer(data_size_bytes)
{
    buffer_data(data, data_size_bytes);
}

auto Buffer::with_size(usize size_bytes) -> Buffer
{
    return Buffer{ size_bytes };
}

auto Buffer::with_data(const void* data, usize data_size_bytes) -> Buffer
{
    return Buffer{ data, data_size_bytes };
}

Buffer::Buffer(const Buffer& other) : Buffer(other._size)
{
    std::memcpy(_data, other._data, _size);
}

auto Buffer::operator=(const Buffer& other) -> Buffer&
{
    if (this == &other)
        return *this;

    if (_size != other._size)
    {
        free();
        allocate(other._size);
    }

    std::memcpy(_data, other._data, _size);
    return *this;
}

Buffer::Buffer(Buffer&& other) noexcept
    : _size(std::exchange(other._size, 0)), _data(std::exchange(other._data, nullptr))
{}

auto Buffer::operator=(Buffer&& other) noexcept -> Buffer&
{
    free();

    _size = std::exchange(other._size, 0);
    _data = std::exchange(other._data, nullptr);

    return *this;
}

Buffer::~Buffer()
{
    free();
}

auto Buffer::data() && -> byte*
{
    _size = 0;
    return std::exchange(_data, nullptr);
}

auto Buffer::operator[](usize offset) -> byte&
{
    ZTH_ASSERT(offset < _size);
    return _data[offset];
}

auto Buffer::operator[](usize offset) const -> const byte&
{
    ZTH_ASSERT(offset < _size);
    return _data[offset];
}

auto Buffer::begin() -> byte*
{
    return cbegin();
}

auto Buffer::begin() const -> const byte*
{
    return cbegin();
}

auto Buffer::cbegin() -> byte*
{
    return _data;
}

auto Buffer::cbegin() const -> const byte*
{
    return _data;
}

auto Buffer::end() -> byte*
{
    return cend();
}

auto Buffer::end() const -> const byte*
{
    return cend();
}

auto Buffer::cend() -> byte*
{
    return cbegin() + _size;
}

auto Buffer::cend() const -> const byte*
{
    return cbegin() + _size;
}

auto Buffer::resize(usize size_bytes) -> void
{
    if (_size == size_bytes)
        return;

    reallocate(size_bytes);
}

auto Buffer::free() -> void
{
    _size = 0;
    zth::free(_data);
}

auto Buffer::buffer_data(const void* data, usize data_size_bytes, usize offset) -> usize
{
    ZTH_ASSERT(offset + data_size_bytes <= _size);
    std::memcpy(_data + offset, data, data_size_bytes);
    return data_size_bytes;
}

auto Buffer::allocate(usize size_bytes) -> void
{
    _data = static_cast<byte*>(zth::allocate(size_bytes));
    _size = size_bytes;
}

auto Buffer::reallocate(usize size_bytes) -> void
{
    zth::reallocate(_data, size_bytes, _size);
    _size = size_bytes;
}

DynamicBuffer::DynamicBuffer(usize size)
{
    resize(size);
}

DynamicBuffer::DynamicBuffer(const void* data, usize data_size_bytes) : DynamicBuffer(data_size_bytes)
{
    buffer_data(data, data_size_bytes);
}

auto DynamicBuffer::with_size(usize size_bytes) -> DynamicBuffer
{
    return DynamicBuffer{ size_bytes };
}

auto DynamicBuffer::with_data(const void* data, usize data_size_bytes) -> DynamicBuffer
{
    return DynamicBuffer{ data, data_size_bytes };
}

DynamicBuffer::DynamicBuffer(const DynamicBuffer& other) : DynamicBuffer(other._size)
{
    std::memcpy(_data, other._data, _size);
}

auto DynamicBuffer::operator=(const DynamicBuffer& other) -> DynamicBuffer&
{
    if (this == &other)
        return *this;

    if (_size < other._size)
    {
        free();
        allocate(other._size);
    }

    _size = other._size;
    std::memcpy(_data, other._data, _size);

    return *this;
}

DynamicBuffer::DynamicBuffer(DynamicBuffer&& other) noexcept
    : _size(std::exchange(other._size, 0)), _capacity(std::exchange(other._capacity, 0)),
      _data(std::exchange(other._data, nullptr))
{}

auto DynamicBuffer::operator=(DynamicBuffer&& other) noexcept -> DynamicBuffer&
{
    free();

    _size = std::exchange(other._size, 0);
    _capacity = std::exchange(other._capacity, 0);
    _data = std::exchange(other._data, nullptr);

    return *this;
}

DynamicBuffer::~DynamicBuffer()
{
    free();
}

auto DynamicBuffer::data() && -> byte*
{
    _size = 0;
    _capacity = 0;
    return std::exchange(_data, nullptr);
}

auto DynamicBuffer::operator[](usize offset) -> byte&
{
    ZTH_ASSERT(offset < _size);
    return _data[offset];
}

auto DynamicBuffer::operator[](usize offset) const -> const byte&
{
    ZTH_ASSERT(offset < _size);
    return _data[offset];
}

auto DynamicBuffer::begin() -> byte*
{
    return cbegin();
}

auto DynamicBuffer::begin() const -> const byte*
{
    return cbegin();
}

auto DynamicBuffer::cbegin() -> byte*
{
    return _data;
}

auto DynamicBuffer::cbegin() const -> const byte*
{
    return _data;
}

auto DynamicBuffer::end() -> byte*
{
    return cend();
}

auto DynamicBuffer::end() const -> const byte*
{
    return cend();
}

auto DynamicBuffer::cend() -> byte*
{
    return cbegin() + _size;
}

auto DynamicBuffer::cend() const -> const byte*
{
    return cbegin() + _size;
}

auto DynamicBuffer::resize(usize size_bytes) -> void
{
    reserve(size_bytes);
    _size = size_bytes;
}

auto DynamicBuffer::reserve(usize min_capacity_bytes) -> void
{
    reallocate_at_least(min_capacity_bytes);
}

auto DynamicBuffer::shrink_to_fit() -> void
{
    ZTH_ASSERT(_size <= _capacity);

    if (_size == _capacity)
        return;

    reallocate_exactly(_size);
}

auto DynamicBuffer::free() -> void
{
    _size = 0;
    _capacity = 0;
    zth::free(_data);
}

auto DynamicBuffer::buffer_data(const void* data, usize data_size_bytes, usize offset) -> usize
{
    resize_to_at_least(offset + data_size_bytes);
    std::memcpy(_data + offset, data, data_size_bytes);
    return data_size_bytes;
}

auto DynamicBuffer::clear() -> void
{
    _size = 0;
}

auto DynamicBuffer::resize_to_at_least(usize min_size_bytes) -> void
{
    reserve(min_size_bytes);
    _size = std::max(_size, min_size_bytes);
}

auto DynamicBuffer::allocate(usize capacity_bytes) -> void
{
    _data = static_cast<byte*>(zth::allocate(capacity_bytes));
    _capacity = capacity_bytes;
}

auto DynamicBuffer::reallocate_exactly(usize new_capacity_bytes) -> void
{
    zth::reallocate(_data, new_capacity_bytes, _size);
    _capacity = new_capacity_bytes;
}

auto DynamicBuffer::reallocate_at_least(usize min_capacity_bytes) -> void
{
    if (_capacity >= min_capacity_bytes)
        return;

    auto new_capacity = _capacity * growth_factor + 1u;

    // @cleanup: could maybe do this a cleaner way
    while (new_capacity < min_capacity_bytes)
        new_capacity *= growth_factor;

    reallocate_exactly(new_capacity);
}

} // namespace zth
