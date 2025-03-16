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
    explicit StaticBuffer(const void* data, size_type data_size_bytes);
    explicit StaticBuffer(std::ranges::contiguous_range auto&& data);

    [[nodiscard]] static auto with_data(const void* data, size_type data_size_bytes) -> StaticBuffer;
    [[nodiscard]] static auto with_data(auto&& data) -> StaticBuffer;
    [[nodiscard]] static auto with_data(std::ranges::contiguous_range auto&& data) -> StaticBuffer;

    ZTH_DEFAULT_COPY_DEFAULT_MOVE(StaticBuffer)

    ~StaticBuffer() = default;

    // --- ContiguousRange implementation
    [[nodiscard]] auto data(this auto&& self) -> auto*;
    [[nodiscard]] auto begin(this auto&& self) -> decltype(auto);
    [[nodiscard]] auto end(this auto&& self) -> decltype(auto);
    [[nodiscard]] auto cbegin() const -> const_iterator;
    [[nodiscard]] auto cend() const -> const_iterator;
    [[nodiscard]] static auto size() -> size_type { return Size; }
    [[nodiscard]] static auto capacity() -> size_type { return Size; }

    // --- StaticBuffer implementation
    // Returns the number of bytes written.
    auto buffer_data(const void* data, size_type data_size_bytes, size_type offset = 0) -> size_type;
    // Returns the number of bytes written.
    auto buffer_data(auto&& data, size_type offset = 0) -> size_type;
    // Returns the number of bytes written.
    auto buffer_data(std::ranges::contiguous_range auto&& data, size_type offset = 0) -> size_type;
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
    explicit Buffer(size_type size_bytes);
    explicit Buffer(const void* data, size_type data_size_bytes);
    explicit Buffer(std::ranges::contiguous_range auto&& data);

    [[nodiscard]] static auto with_size(size_type size_bytes) -> Buffer;
    [[nodiscard]] static auto with_data(const void* data, size_type data_size_bytes) -> Buffer;
    [[nodiscard]] static auto with_data(auto&& data) -> Buffer;
    [[nodiscard]] static auto with_data(std::ranges::contiguous_range auto&& data) -> Buffer;

    Buffer(const Buffer& other);
    auto operator=(const Buffer& other) -> Buffer&;

    Buffer(Buffer&& other) noexcept;
    auto operator=(Buffer&& other) noexcept -> Buffer&;

    ~Buffer();

    // --- ContiguousRange implementation
    [[nodiscard]] auto data(this auto&& self) -> auto*;
    [[nodiscard]] auto begin(this auto&& self) -> decltype(auto);
    [[nodiscard]] auto end(this auto&& self) -> decltype(auto);
    [[nodiscard]] auto cbegin() const -> const_iterator;
    [[nodiscard]] auto cend() const -> const_iterator;
    [[nodiscard]] auto size() const -> size_type { return _size_bytes; }
    [[nodiscard]] auto capacity() const -> size_type { return _size_bytes; }

    // --- Buffer implementation
    auto resize(size_type size_bytes) -> void;
    auto free() -> void;

    // Returns the number of bytes written.
    auto buffer_data(const void* data, size_type data_size_bytes, size_type offset = 0) -> size_type;
    // Returns the number of bytes written.
    auto buffer_data(auto&& data, size_type offset = 0) -> size_type;
    // Returns the number of bytes written.
    auto buffer_data(std::ranges::contiguous_range auto&& data, size_type offset = 0) -> size_type;

private:
    size_type _size_bytes = 0;
    byte* _data = nullptr; // We assume that _data is always properly aligned to point to any type.
    // We're not using unique_ptr because a unique_ptr makes it more messy to reallocate the data.

private:
    auto allocate(size_type size_bytes) -> void;
    auto reallocate(size_type size_bytes) -> void;
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
    explicit DynamicBuffer(size_type size_bytes);
    explicit DynamicBuffer(const void* data, size_type data_size_bytes);
    explicit DynamicBuffer(std::ranges::contiguous_range auto&& data);

    [[nodiscard]] static auto with_size(size_type size_bytes) -> DynamicBuffer;
    [[nodiscard]] static auto with_data(const void* data, size_type data_size_bytes) -> DynamicBuffer;
    [[nodiscard]] static auto with_data(auto&& data) -> DynamicBuffer;
    [[nodiscard]] static auto with_data(std::ranges::contiguous_range auto&& data) -> DynamicBuffer;

    DynamicBuffer(const DynamicBuffer& other);
    auto operator=(const DynamicBuffer& other) -> DynamicBuffer&;

    DynamicBuffer(DynamicBuffer&& other) noexcept;
    auto operator=(DynamicBuffer&& other) noexcept -> DynamicBuffer&;

    ~DynamicBuffer();

    // --- ContiguousRange implementation
    [[nodiscard]] auto data(this auto&& self) -> auto*;
    [[nodiscard]] auto begin(this auto&& self) -> decltype(auto);
    [[nodiscard]] auto end(this auto&& self) -> decltype(auto);
    [[nodiscard]] auto cbegin() const -> const_iterator;
    [[nodiscard]] auto cend() const -> const_iterator;
    [[nodiscard]] auto size() const -> size_type { return _size_bytes; }
    [[nodiscard]] auto capacity() const -> size_type { return _capacity_bytes; }

    // --- DynamicBuffer implementation
    auto resize(size_type size_bytes) -> void;
    auto reserve(size_type min_capacity_bytes) -> void;
    auto shrink_to_fit() -> void;
    auto free() -> void;

    // Returns the number of bytes written.
    auto buffer_data(const void* data, size_type data_size_bytes, size_type offset = 0) -> size_type;
    // Returns the number of bytes written.
    auto buffer_data(auto&& data, size_type offset = 0) -> size_type;
    // Returns the number of bytes written.
    auto buffer_data(std::ranges::contiguous_range auto&& data, size_type offset = 0) -> size_type;

    auto clear() -> void;

private:
    size_type _size_bytes = 0;
    size_type _capacity_bytes = 0;
    byte* _data = nullptr; // We assume that _data is always properly aligned to point to any type.
    // We're not using unique_ptr because a unique_ptr makes it more messy to reallocate the data.

private:
    auto resize_to_at_least(size_type min_size_bytes) -> void;

    auto allocate(size_type capacity_bytes) -> void;
    auto reallocate_exactly(size_type new_capacity_bytes) -> void;
    auto reallocate_at_least(size_type min_capacity_bytes) -> void;

    [[nodiscard]] static auto calculate_growth(size_type old_size_bytes) -> size_type;
};

} // namespace zth::memory

#include "buffer.inl"
