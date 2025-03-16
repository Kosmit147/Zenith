#pragma once

#include <array>
#include <ranges>

#include "zenith/core/typedefs.hpp"
#include "zenith/memory/fwd.hpp"
#include "zenith/stl/range.hpp"
#include "zenith/util/macros.hpp"
#include "zenith/util/optional.hpp"
#include "zenith/util/reference.hpp"

namespace zth::memory {

// @todo: Implement iterators.
// @todo: Implement reverse iterators (rbegin, rend).

template<usize Size, usize Alignment /* = minimal_alignment */>
class alignas(Alignment) StaticBuffer : public ContiguousRangeInterface
{
public:
    using UnderlyingStorage = std::array<byte, Size>;

    using value_type = byte;
    using size_type = usize;
    using difference_type = isize;

    using pointer = value_type*;
    using const_pointer = const value_type*;

    using iterator = typename UnderlyingStorage::iterator;
    using const_iterator = typename UnderlyingStorage::const_iterator;

    alignas(Alignment) UnderlyingStorage bytes; // The data is uninitialized.

public:
    explicit StaticBuffer() = default;
    explicit StaticBuffer(const void* data, size_type data_size_bytes) noexcept;
    explicit StaticBuffer(std::ranges::contiguous_range auto&& data) noexcept;

    [[nodiscard]] static auto with_data(const void* data, size_type data_size_bytes) noexcept -> StaticBuffer;
    [[nodiscard]] static auto with_data(auto&& data) noexcept -> StaticBuffer;
    [[nodiscard]] static auto with_data(std::ranges::contiguous_range auto&& data) noexcept -> StaticBuffer;

    ZTH_DEFAULT_COPY_DEFAULT_MOVE(StaticBuffer)

    ~StaticBuffer() = default;

    // --- ContiguousRange implementation
    [[nodiscard]] auto data(this auto&& self) noexcept -> auto*;
    [[nodiscard]] auto begin(this auto&& self) noexcept -> decltype(auto);
    [[nodiscard]] auto end(this auto&& self) noexcept -> decltype(auto);
    [[nodiscard]] auto cbegin() const noexcept -> const_iterator;
    [[nodiscard]] auto cend() const noexcept -> const_iterator;
    [[nodiscard]] static auto size() noexcept -> size_type { return Size; }
    [[nodiscard]] static auto capacity() noexcept -> size_type { return Size; }

    // --- StaticBuffer implementation
    // Returns the number of bytes written.
    auto buffer_data(const void* data, size_type data_size_bytes, size_type offset = 0) noexcept -> size_type;
    // Returns the number of bytes written.
    auto buffer_data(auto&& data, size_type offset = 0) noexcept -> size_type;
    // Returns the number of bytes written.
    auto buffer_data(std::ranges::contiguous_range auto&& data, size_type offset = 0) noexcept -> size_type;
};

class Buffer : public ContiguousRangeInterface
{
public:
    using value_type = byte;
    using size_type = usize;
    using difference_type = isize;

    using pointer = value_type*;
    using const_pointer = const value_type*;

    using iterator = pointer;
    using const_iterator = const_pointer;

    explicit Buffer() = default;
    explicit Buffer(size_type size_bytes) noexcept;
    explicit Buffer(const void* data, size_type data_size_bytes) noexcept;
    explicit Buffer(std::ranges::contiguous_range auto&& data) noexcept;

    [[nodiscard]] static auto with_size(size_type size_bytes) noexcept -> Buffer;
    [[nodiscard]] static auto with_data(const void* data, size_type data_size_bytes) noexcept -> Buffer;
    [[nodiscard]] static auto with_data(auto&& data) noexcept -> Buffer;
    [[nodiscard]] static auto with_data(std::ranges::contiguous_range auto&& data) noexcept -> Buffer;

    Buffer(const Buffer& other) noexcept;
    auto operator=(const Buffer& other) noexcept -> Buffer&;

    Buffer(Buffer&& other) noexcept;
    auto operator=(Buffer&& other) noexcept -> Buffer&;

    ~Buffer();

    // --- ContiguousRange implementation
    [[nodiscard]] auto data(this auto&& self) noexcept -> auto*;
    [[nodiscard]] auto begin(this auto&& self) noexcept -> decltype(auto);
    [[nodiscard]] auto end(this auto&& self) noexcept -> decltype(auto);
    [[nodiscard]] auto cbegin() const noexcept -> const_iterator;
    [[nodiscard]] auto cend() const noexcept -> const_iterator;
    [[nodiscard]] auto size() const noexcept -> size_type { return _size_bytes; }
    [[nodiscard]] auto capacity() const noexcept -> size_type { return _size_bytes; }

    // --- Buffer implementation
    auto resize(size_type size_bytes) noexcept -> void;
    auto free() noexcept -> void;

    // Returns the number of bytes written.
    auto buffer_data(const void* data, size_type data_size_bytes, size_type offset = 0) noexcept -> size_type;
    // Returns the number of bytes written.
    auto buffer_data(auto&& data, size_type offset = 0) noexcept -> size_type;
    // Returns the number of bytes written.
    auto buffer_data(std::ranges::contiguous_range auto&& data, size_type offset = 0) noexcept -> size_type;

private:
    size_type _size_bytes = 0;
    byte* _data = nullptr; // We assume that _data is always properly aligned to point to any type.
    // We're not using unique_ptr because a unique_ptr makes it more messy to reallocate the data.

private:
    auto allocate(size_type size_bytes) noexcept -> void;
    auto reallocate(size_type size_bytes) noexcept -> void;
};

class DynamicBuffer : public ContiguousRangeInterface
{
public:
    using value_type = byte;
    using size_type = usize;
    using difference_type = isize;

    using pointer = value_type*;
    using const_pointer = const value_type*;

    using iterator = pointer;
    using const_iterator = const_pointer;

    explicit DynamicBuffer() = default;
    explicit DynamicBuffer(size_type size_bytes) noexcept;
    explicit DynamicBuffer(const void* data, size_type data_size_bytes) noexcept;
    explicit DynamicBuffer(std::ranges::contiguous_range auto&& data) noexcept;

    [[nodiscard]] static auto with_size(size_type size_bytes) noexcept -> DynamicBuffer;
    [[nodiscard]] static auto with_data(const void* data, size_type data_size_bytes) noexcept -> DynamicBuffer;
    [[nodiscard]] static auto with_data(auto&& data) noexcept -> DynamicBuffer;
    [[nodiscard]] static auto with_data(std::ranges::contiguous_range auto&& data) noexcept -> DynamicBuffer;

    DynamicBuffer(const DynamicBuffer& other) noexcept;
    auto operator=(const DynamicBuffer& other) noexcept -> DynamicBuffer&;

    DynamicBuffer(DynamicBuffer&& other) noexcept;
    auto operator=(DynamicBuffer&& other) noexcept -> DynamicBuffer&;

    ~DynamicBuffer();

    // --- ContiguousRange implementation
    [[nodiscard]] auto data(this auto&& self) noexcept -> auto*;
    [[nodiscard]] auto begin(this auto&& self) noexcept -> decltype(auto);
    [[nodiscard]] auto end(this auto&& self) noexcept -> decltype(auto);
    [[nodiscard]] auto cbegin() const noexcept -> const_iterator;
    [[nodiscard]] auto cend() const noexcept -> const_iterator;
    [[nodiscard]] auto size() const noexcept -> size_type { return _size_bytes; }
    [[nodiscard]] auto capacity() const noexcept -> size_type { return _capacity_bytes; }

    // --- DynamicBuffer implementation
    auto resize(size_type size_bytes) noexcept -> void;
    auto reserve(size_type min_capacity_bytes) noexcept -> void;
    auto shrink_to_fit() noexcept -> void;
    auto free() noexcept -> void;

    // Returns the number of bytes written.
    auto buffer_data(const void* data, size_type data_size_bytes, size_type offset = 0) noexcept -> size_type;
    // Returns the number of bytes written.
    auto buffer_data(auto&& data, size_type offset = 0) noexcept -> size_type;
    // Returns the number of bytes written.
    auto buffer_data(std::ranges::contiguous_range auto&& data, size_type offset = 0) noexcept -> size_type;

    auto clear() noexcept -> void;

private:
    size_type _size_bytes = 0;
    size_type _capacity_bytes = 0;
    byte* _data = nullptr; // We assume that _data is always properly aligned to point to any type.
    // We're not using unique_ptr because a unique_ptr makes it more messy to reallocate the data.

private:
    auto resize_to_at_least(size_type min_size_bytes) noexcept -> void;

    auto allocate(size_type capacity_bytes) noexcept -> void;
    auto reallocate_exactly(size_type new_capacity_bytes) noexcept -> void;
    auto reallocate_at_least(size_type min_capacity_bytes) noexcept -> void;

    [[nodiscard]] static auto calculate_growth(size_type old_size_bytes) noexcept -> size_type;
};

// @todo: Implement SmallBuffer.
template<usize StackCapacity> using SmallBuffer = Buffer;

// @todo: Implement SmallDynamicBuffer.
template<usize StackCapacity> using SmallDynamicBuffer = DynamicBuffer;

} // namespace zth::memory

#include "buffer.inl"
