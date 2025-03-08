#pragma once

#include <memory>

namespace zth::memory {

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

auto Buffer::buffer_data(auto&& data, usize offset) -> usize
{
    return buffer_data(std::addressof(data), sizeof(data), offset);
}

auto Buffer::buffer_data(std::ranges::contiguous_range auto&& data, usize offset) -> usize
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

auto DynamicBuffer::buffer_data(auto&& data, usize offset) -> usize
{
    return buffer_data(std::addressof(data), sizeof(data), offset);
}

auto DynamicBuffer::buffer_data(std::ranges::contiguous_range auto&& data, usize offset) -> usize
{
    using T = std::ranges::range_value_t<decltype(data)>;
    auto data_size_bytes = std::size(data) * sizeof(T);
    return buffer_data(std::data(data), data_size_bytes, offset);
}

} // namespace zth::memory
