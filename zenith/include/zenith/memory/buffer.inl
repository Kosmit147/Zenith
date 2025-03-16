#pragma once

#include <cstring>
#include <iterator>
#include <memory>
#include <type_traits>

#include "zenith/core/assert.hpp"

namespace zth::memory {

template<usize Size, usize Alignment>
StaticBuffer<Size, Alignment>::StaticBuffer(const void* data, size_type data_size_bytes)
{
    buffer_data(data, data_size_bytes);
}

template<usize Size, usize Alignment>
StaticBuffer<Size, Alignment>::StaticBuffer(std::ranges::contiguous_range auto&& data)
    : StaticBuffer(std::data(data), std::size(data) * sizeof(std::ranges::range_value_t<decltype(data)>))
{}

template<usize Size, usize Alignment>
auto StaticBuffer<Size, Alignment>::with_data(const void* data, size_type data_size_bytes) -> StaticBuffer
{
    return StaticBuffer{ data, data_size_bytes };
}

template<usize Size, usize Alignment> auto StaticBuffer<Size, Alignment>::with_data(auto&& data) -> StaticBuffer
{
    return StaticBuffer{ std::addressof(data), sizeof(data) };
}

template<usize Size, usize Alignment>
auto StaticBuffer<Size, Alignment>::with_data(std::ranges::contiguous_range auto&& data) -> StaticBuffer
{
    return StaticBuffer{ data };
}

template<usize Size, usize Alignment> auto StaticBuffer<Size, Alignment>::data(this auto&& self) -> auto*
{
    return self.bytes.data();
}

template<usize Size, usize Alignment> auto StaticBuffer<Size, Alignment>::begin(this auto&& self) -> decltype(auto)
{
    return self.bytes.begin();
}

template<usize Size, usize Alignment> auto StaticBuffer<Size, Alignment>::end(this auto&& self) -> decltype(auto)
{
    return self.bytes.end();
}

template<usize Size, usize Alignment> auto StaticBuffer<Size, Alignment>::cbegin() const -> const_iterator
{
    return bytes.cbegin();
}

template<usize Size, usize Alignment> auto StaticBuffer<Size, Alignment>::cend() const -> const_iterator
{
    return bytes.cend();
}

template<usize Size, usize Alignment>
auto StaticBuffer<Size, Alignment>::buffer_data(const void* data, size_type data_size_bytes, size_type offset)
    -> size_type
{
    ZTH_ASSERT(offset + data_size_bytes <= Size);
    std::memcpy(this->data() + offset, data, data_size_bytes);
    return data_size_bytes;
}

template<usize Size, usize Alignment>
auto StaticBuffer<Size, Alignment>::buffer_data(auto&& data, size_type offset) -> size_type
{
    return buffer_data(std::addressof(data), sizeof(data), offset);
}

template<usize Size, usize Alignment>
auto StaticBuffer<Size, Alignment>::buffer_data(std::ranges::contiguous_range auto&& data, size_type offset)
    -> size_type
{
    using T = std::ranges::range_value_t<decltype(data)>;
    auto data_size_bytes = std::size(data) * sizeof(T);
    return buffer_data(std::data(data), data_size_bytes, offset);
}

Buffer::Buffer(std::ranges::contiguous_range auto&& data)
    : Buffer(std::data(data), std::size(data) * sizeof(std::ranges::range_value_t<decltype(data)>))
{}

auto Buffer::with_data(auto&& data) -> Buffer
{
    return Buffer{ std::addressof(data), sizeof(data) };
}

auto Buffer::with_data(std::ranges::contiguous_range auto&& data) -> Buffer
{
    return Buffer{ data };
}

auto Buffer::data(this auto&& self) -> auto*
{
    using return_type =
        std::conditional_t<std::is_const_v<std::remove_reference_t<decltype(self)>>, const_pointer, pointer>;

    return static_cast<return_type>(self._data);
}

auto Buffer::begin(this auto&& self) -> decltype(auto)
{
    return self.data();
}

auto Buffer::end(this auto&& self) -> decltype(auto)
{
    return std::next(self.begin(), self.size());
}

auto Buffer::buffer_data(auto&& data, size_type offset) -> size_type
{
    return buffer_data(std::addressof(data), sizeof(data), offset);
}

auto Buffer::buffer_data(std::ranges::contiguous_range auto&& data, size_type offset) -> size_type
{
    using T = std::ranges::range_value_t<decltype(data)>;
    auto data_size_bytes = std::size(data) * sizeof(T);
    return buffer_data(std::data(data), data_size_bytes, offset);
}

DynamicBuffer::DynamicBuffer(std::ranges::contiguous_range auto&& data)
    : DynamicBuffer(std::data(data), std::size(data) * sizeof(std::ranges::range_value_t<decltype(data)>))
{}

auto DynamicBuffer::with_data(auto&& data) -> DynamicBuffer
{
    return DynamicBuffer{ std::addressof(data), sizeof(data) };
}

auto DynamicBuffer::with_data(std::ranges::contiguous_range auto&& data) -> DynamicBuffer
{
    return DynamicBuffer{ data };
}

auto DynamicBuffer::data(this auto&& self) -> auto*
{
    using return_type =
        std::conditional_t<std::is_const_v<std::remove_reference_t<decltype(self)>>, const_pointer, pointer>;

    return static_cast<return_type>(self._data);
}

auto DynamicBuffer::begin(this auto&& self) -> decltype(auto)
{
    return self.data();
}

auto DynamicBuffer::end(this auto&& self) -> decltype(auto)
{
    return std::next(self.begin(), self.size());
}

auto DynamicBuffer::buffer_data(auto&& data, size_type offset) -> size_type
{
    return buffer_data(std::addressof(data), sizeof(data), offset);
}

auto DynamicBuffer::buffer_data(std::ranges::contiguous_range auto&& data, size_type offset) -> size_type
{
    using T = std::ranges::range_value_t<decltype(data)>;
    auto data_size_bytes = std::size(data) * sizeof(T);
    return buffer_data(std::data(data), data_size_bytes, offset);
}

} // namespace zth::memory
