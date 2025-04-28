#pragma once

#include <algorithm>
#include <cstring>
#include <iterator>
#include <utility>

#include "zenith/core/assert.hpp"

namespace zth::memory {

template<usize Size, usize Alignment>
StaticBuffer<Size, Alignment>::StaticBuffer(const void* data, size_type data_size_bytes) noexcept
{
    buffer_data(data, data_size_bytes);
}

template<usize Size, usize Alignment>
StaticBuffer<Size, Alignment>::StaticBuffer(std::ranges::contiguous_range auto&& data) noexcept
    : StaticBuffer(std::data(data), std::size(data) * sizeof(std::ranges::range_value_t<decltype(data)>))
{}

template<usize Size, usize Alignment>
auto StaticBuffer<Size, Alignment>::with_data(const void* data, size_type data_size_bytes) noexcept -> StaticBuffer
{
    return StaticBuffer{ data, data_size_bytes };
}

template<usize Size, usize Alignment>
auto StaticBuffer<Size, Alignment>::with_data(auto&& data) noexcept -> StaticBuffer
{
    return StaticBuffer{ std::addressof(data), sizeof(data) };
}

template<usize Size, usize Alignment>
auto StaticBuffer<Size, Alignment>::with_data(std::ranges::contiguous_range auto&& data) noexcept -> StaticBuffer
{
    return StaticBuffer{ data };
}

template<usize Size, usize Alignment> auto StaticBuffer<Size, Alignment>::data(this auto&& self) noexcept -> auto*
{
    return self.bytes.data();
}

template<usize Size, usize Alignment>
auto StaticBuffer<Size, Alignment>::begin(this auto&& self) noexcept -> decltype(auto)
{
    return self.bytes.begin();
}

template<usize Size, usize Alignment>
auto StaticBuffer<Size, Alignment>::end(this auto&& self) noexcept -> decltype(auto)
{
    return self.bytes.end();
}

template<usize Size, usize Alignment> auto StaticBuffer<Size, Alignment>::cbegin() const noexcept -> const_iterator
{
    return bytes.cbegin();
}

template<usize Size, usize Alignment> auto StaticBuffer<Size, Alignment>::cend() const noexcept -> const_iterator
{
    return bytes.cend();
}

template<usize Size, usize Alignment>
auto StaticBuffer<Size, Alignment>::buffer_data(const void* data, size_type data_size_bytes, size_type offset) noexcept
    -> size_type
{
    ZTH_ASSERT(offset + data_size_bytes <= Size);
    std::memcpy(this->data() + offset, data, data_size_bytes);
    return data_size_bytes;
}

template<usize Size, usize Alignment>
auto StaticBuffer<Size, Alignment>::buffer_data(auto&& data, size_type offset) noexcept -> size_type
{
    return buffer_data(std::addressof(data), sizeof(data), offset);
}

template<usize Size, usize Alignment>
auto StaticBuffer<Size, Alignment>::buffer_data(std::ranges::contiguous_range auto&& data, size_type offset) noexcept
    -> size_type
{
    using T = std::ranges::range_value_t<decltype(data)>;
    auto data_size_bytes = std::size(data) * sizeof(T);
    return buffer_data(std::data(data), data_size_bytes, offset);
}

template<StatelessAllocator A> Buffer<A>::Buffer(size_type size_bytes) noexcept
{
    allocate(size_bytes);
}

template<StatelessAllocator A>
Buffer<A>::Buffer(const void* data, size_type data_size_bytes) noexcept : Buffer(data_size_bytes)
{
    buffer_data(data, data_size_bytes);
}

template<StatelessAllocator A>
Buffer<A>::Buffer(std::ranges::contiguous_range auto&& data) noexcept
    : Buffer(std::data(data), std::size(data) * sizeof(std::ranges::range_value_t<decltype(data)>))
{}

template<StatelessAllocator A> auto Buffer<A>::with_size(size_type size_bytes) noexcept -> Buffer
{
    return Buffer{ size_bytes };
}

template<StatelessAllocator A> auto Buffer<A>::with_data(const void* data, size_type data_size_bytes) noexcept -> Buffer
{
    return Buffer{ data, data_size_bytes };
}

template<StatelessAllocator A> auto Buffer<A>::with_data(auto&& data) noexcept -> Buffer
{
    return Buffer{ std::addressof(data), sizeof(data) };
}

template<StatelessAllocator A> auto Buffer<A>::with_data(std::ranges::contiguous_range auto&& data) noexcept -> Buffer
{
    return Buffer{ data };
}

template<StatelessAllocator A> Buffer<A>::Buffer(const Buffer& other) noexcept : Buffer(other._size_bytes)
{
    std::memcpy(_data, other._data, _size_bytes);
}

template<StatelessAllocator A> auto Buffer<A>::operator=(const Buffer& other) noexcept -> Buffer&
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

template<StatelessAllocator A>
Buffer<A>::Buffer(Buffer&& other) noexcept
    : _size_bytes(std::exchange(other._size_bytes, 0)), _data(std::exchange(other._data, nullptr))
{}

template<StatelessAllocator A> auto Buffer<A>::operator=(Buffer&& other) noexcept -> Buffer&
{
    free();

    _size_bytes = std::exchange(other._size_bytes, 0);
    _data = std::exchange(other._data, nullptr);

    return *this;
}

template<StatelessAllocator A> Buffer<A>::~Buffer()
{
    free();
}

template<StatelessAllocator A> auto Buffer<A>::data() noexcept -> pointer
{
    return _data;
}

template<StatelessAllocator A> auto Buffer<A>::data() const noexcept -> const_pointer
{
    return _data;
}

template<StatelessAllocator A> auto Buffer<A>::begin(this auto&& self) noexcept -> decltype(auto)
{
    return self.data();
}

template<StatelessAllocator A> auto Buffer<A>::end(this auto&& self) noexcept -> decltype(auto)
{
    return std::next(self.begin(), self.size());
}

template<StatelessAllocator A> auto Buffer<A>::cbegin() const noexcept -> const_iterator
{
    return begin();
}

template<StatelessAllocator A> auto Buffer<A>::cend() const noexcept -> const_iterator
{
    return end();
}

template<StatelessAllocator A> auto Buffer<A>::resize(size_type size_bytes) noexcept -> void
{
    if (_size_bytes == size_bytes)
        return;

    reallocate(size_bytes);
}

template<StatelessAllocator A> auto Buffer<A>::resize_to_at_least(size_type min_size_bytes) noexcept -> void
{
    resize(std::max(_size_bytes, min_size_bytes));
}

template<StatelessAllocator A> auto Buffer<A>::free() noexcept -> void
{
    destroy_and_deallocate_objects_using_allocator(A{}, _data, _size_bytes);
    _size_bytes = 0;
    _data = nullptr;
}

template<StatelessAllocator A>
auto Buffer<A>::buffer_data(const void* data, size_type data_size_bytes, size_type offset) noexcept -> size_type
{
    ZTH_ASSERT(offset + data_size_bytes <= _size_bytes);
    std::memcpy(_data + offset, data, data_size_bytes);
    return data_size_bytes;
}

template<StatelessAllocator A> auto Buffer<A>::buffer_data(auto&& data, size_type offset) noexcept -> size_type
{
    return buffer_data(std::addressof(data), sizeof(data), offset);
}

template<StatelessAllocator A>
auto Buffer<A>::buffer_data(std::ranges::contiguous_range auto&& data, size_type offset) noexcept -> size_type
{
    using T = std::ranges::range_value_t<decltype(data)>;
    auto data_size_bytes = std::size(data) * sizeof(T);
    return buffer_data(std::data(data), data_size_bytes, offset);
}

template<StatelessAllocator A> auto Buffer<A>::allocate(size_type size_bytes) noexcept -> void
{
    _data = allocate_using_allocator(A{}, size_bytes); // We deliberately don't construct any objects.

#if defined(ZTH_ASSERTIONS)
    if (size_bytes != 0)
        ZTH_ASSERT(_data != nullptr);
#endif

    _size_bytes = size_bytes;
}

template<StatelessAllocator A> auto Buffer<A>::reallocate(size_type size_bytes) noexcept -> void
{
    auto* new_data = allocate_using_allocator(A{}, size_bytes); // We deliberately don't construct any objects.

#if defined(ZTH_ASSERTIONS)
    if (size_bytes != 0)
        ZTH_ASSERT(new_data != nullptr);
#endif

    std::memcpy(new_data, _data, std::min(_size_bytes, size_bytes));
    destroy_and_deallocate_objects_using_allocator(A{}, _data, _size_bytes);
    _size_bytes = size_bytes;
}

template<StatelessAllocator A> DynamicBuffer<A>::DynamicBuffer(size_type size_bytes) noexcept
{
    resize(size_bytes);
}

template<StatelessAllocator A>
DynamicBuffer<A>::DynamicBuffer(const void* data, size_type data_size_bytes) noexcept : DynamicBuffer(data_size_bytes)
{
    buffer_data(data, data_size_bytes);
}

template<StatelessAllocator A>
DynamicBuffer<A>::DynamicBuffer(std::ranges::contiguous_range auto&& data) noexcept
    : DynamicBuffer(std::data(data), std::size(data) * sizeof(std::ranges::range_value_t<decltype(data)>))
{}

template<StatelessAllocator A> auto DynamicBuffer<A>::with_size(size_type size_bytes) noexcept -> DynamicBuffer
{
    return DynamicBuffer{ size_bytes };
}

template<StatelessAllocator A>
auto DynamicBuffer<A>::with_data(const void* data, size_type data_size_bytes) noexcept -> DynamicBuffer
{
    return DynamicBuffer{ data, data_size_bytes };
}

template<StatelessAllocator A> auto DynamicBuffer<A>::with_data(auto&& data) noexcept -> DynamicBuffer
{
    return DynamicBuffer{ std::addressof(data), sizeof(data) };
}

template<StatelessAllocator A>
auto DynamicBuffer<A>::with_data(std::ranges::contiguous_range auto&& data) noexcept -> DynamicBuffer
{
    return DynamicBuffer{ data };
}

template<StatelessAllocator A>
DynamicBuffer<A>::DynamicBuffer(const DynamicBuffer& other) noexcept : DynamicBuffer(other._size_bytes)
{
    std::memcpy(_data, other._data, _size_bytes);
}

template<StatelessAllocator A> auto DynamicBuffer<A>::operator=(const DynamicBuffer& other) noexcept -> DynamicBuffer&
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

template<StatelessAllocator A>
DynamicBuffer<A>::DynamicBuffer(DynamicBuffer&& other) noexcept
    : _size_bytes(std::exchange(other._size_bytes, 0)), _capacity_bytes(std::exchange(other._capacity_bytes, 0)),
      _data(std::exchange(other._data, nullptr))
{}

template<StatelessAllocator A> auto DynamicBuffer<A>::operator=(DynamicBuffer&& other) noexcept -> DynamicBuffer&
{
    free();

    _size_bytes = std::exchange(other._size_bytes, 0);
    _capacity_bytes = std::exchange(other._capacity_bytes, 0);
    _data = std::exchange(other._data, nullptr);

    return *this;
}

template<StatelessAllocator A> DynamicBuffer<A>::~DynamicBuffer()
{
    free();
}

template<StatelessAllocator A> auto DynamicBuffer<A>::data() noexcept -> pointer
{
    return _data;
}

template<StatelessAllocator A> auto DynamicBuffer<A>::data() const noexcept -> const_pointer
{
    return _data;
}

template<StatelessAllocator A> auto DynamicBuffer<A>::begin(this auto&& self) noexcept -> decltype(auto)
{
    return self.data();
}

template<StatelessAllocator A> auto DynamicBuffer<A>::end(this auto&& self) noexcept -> decltype(auto)
{
    return std::next(self.begin(), self.size());
}

template<StatelessAllocator A> auto DynamicBuffer<A>::cbegin() const noexcept -> const_iterator
{
    return begin();
}

template<StatelessAllocator A> auto DynamicBuffer<A>::cend() const noexcept -> const_iterator
{
    return end();
}

template<StatelessAllocator A> auto DynamicBuffer<A>::resize(size_type size_bytes) noexcept -> void
{
    reserve(size_bytes);
    _size_bytes = size_bytes;
}

template<StatelessAllocator A> auto DynamicBuffer<A>::resize_to_at_least(size_type min_size_bytes) noexcept -> void
{
    resize(std::max(_size_bytes, min_size_bytes));
}

template<StatelessAllocator A> auto DynamicBuffer<A>::reserve(size_type min_capacity_bytes) noexcept -> void
{
    reallocate_at_least(min_capacity_bytes);
}

template<StatelessAllocator A> auto DynamicBuffer<A>::shrink_to_fit() noexcept -> void
{
    ZTH_ASSERT(_size_bytes <= _capacity_bytes);

    if (_size_bytes == _capacity_bytes)
        return;

    reallocate_exactly(_size_bytes);
}

template<StatelessAllocator A> auto DynamicBuffer<A>::free() noexcept -> void
{
    destroy_and_deallocate_objects_using_allocator(A{}, _data, _size_bytes);
    _size_bytes = 0;
    _capacity_bytes = 0;
    _data = nullptr;
}

template<StatelessAllocator A>
auto DynamicBuffer<A>::buffer_data(const void* data, size_type data_size_bytes, size_type offset) noexcept -> size_type
{
    resize_to_at_least(offset + data_size_bytes);
    std::memcpy(_data + offset, data, data_size_bytes);
    return data_size_bytes;
}

template<StatelessAllocator A> auto DynamicBuffer<A>::buffer_data(auto&& data, size_type offset) noexcept -> size_type
{
    return buffer_data(std::addressof(data), sizeof(data), offset);
}

template<StatelessAllocator A>
auto DynamicBuffer<A>::buffer_data(std::ranges::contiguous_range auto&& data, size_type offset) noexcept -> size_type
{
    using T = std::ranges::range_value_t<decltype(data)>;
    auto data_size_bytes = std::size(data) * sizeof(T);
    return buffer_data(std::data(data), data_size_bytes, offset);
}

template<StatelessAllocator A> auto DynamicBuffer<A>::clear() noexcept -> void
{
    _size_bytes = 0;
}

template<StatelessAllocator A> auto DynamicBuffer<A>::allocate(size_type capacity_bytes) noexcept -> void
{
    _data = allocate_using_allocator(A{}, capacity_bytes); // We deliberately don't construct any objects.

#if defined(ZTH_ASSERTIONS)
    if (capacity_bytes != 0)
        ZTH_ASSERT(_data != nullptr);
#endif

    _capacity_bytes = capacity_bytes;
}

template<StatelessAllocator A> auto DynamicBuffer<A>::reallocate_exactly(size_type capacity_bytes) noexcept -> void
{
    auto* new_data = allocate_using_allocator(A{}, capacity_bytes); // We deliberately don't construct any objects.

#if defined(ZTH_ASSERTIONS)
    if (capacity_bytes != 0)
        ZTH_ASSERT(new_data != nullptr);
#endif

    std::memcpy(new_data, _data, std::min(_size_bytes, capacity_bytes));
    destroy_and_deallocate_objects_using_allocator(A{}, _data, _capacity_bytes);
    _capacity_bytes = capacity_bytes;
}

template<StatelessAllocator A> auto DynamicBuffer<A>::reallocate_at_least(size_type min_capacity_bytes) noexcept -> void
{
    if (_capacity_bytes >= min_capacity_bytes)
        return;

    auto new_capacity = std::max(calculate_growth(_capacity_bytes), min_capacity_bytes);
    reallocate_exactly(new_capacity);
}

template<StatelessAllocator A> auto DynamicBuffer<A>::calculate_growth(size_type old_size_bytes) noexcept -> size_type
{
    return old_size_bytes + old_size_bytes / 2;
}

static_assert(std::ranges::contiguous_range<StaticBuffer<1>>);
static_assert(std::ranges::contiguous_range<Buffer<>>);
static_assert(std::ranges::contiguous_range<DynamicBuffer<>>);
static_assert(std::ranges::contiguous_range<SmallBuffer<1>>);
static_assert(std::ranges::contiguous_range<SmallDynamicBuffer<1>>);

} // namespace zth::memory
