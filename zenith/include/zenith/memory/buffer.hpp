#pragma once

#include <array>
#include <functional>
#include <optional>
#include <ranges>

#include "zenith/core/typedefs.hpp"
#include "zenith/memory/fwd.hpp"

namespace zth::memory {

template<usize Size, usize Alignment> class alignas(Alignment) StaticBuffer
{
public:
    alignas(Alignment) std::array<byte, Size> data;

    // @todo
};

class Buffer
{
public:
    using ByteRef = std::reference_wrapper<byte>;
    using ConstByteRef = std::reference_wrapper<const byte>;

    explicit Buffer() = default;
    explicit Buffer(usize size_bytes);
    explicit Buffer(const void* data, usize data_size_bytes);
    explicit Buffer(std::ranges::contiguous_range auto&& data);

    [[nodiscard]] static auto with_size(usize size_bytes) -> Buffer;
    [[nodiscard]] static auto with_data(const void* data, usize data_size_bytes) -> Buffer;
    [[nodiscard]] static auto with_data(auto&& data) -> Buffer;
    [[nodiscard]] static auto with_data(std::ranges::contiguous_range auto&& data) -> Buffer;

    Buffer(const Buffer& other);
    auto operator=(const Buffer& other) -> Buffer&;

    Buffer(Buffer&& other) noexcept;
    auto operator=(Buffer&& other) noexcept -> Buffer&;

    ~Buffer();

    // --- Data access
    [[nodiscard]] auto data() & { return _data; }
    [[nodiscard]] auto data() const& -> const auto* { return _data; }
    [[nodiscard]] auto data() && -> byte*; // The returned pointer must be freed!

    [[nodiscard]] auto at(usize offset) -> std::optional<ByteRef>;
    [[nodiscard]] auto at(usize offset) const -> std::optional<ConstByteRef>;

    [[nodiscard]] auto operator[](usize offset) -> byte&;
    [[nodiscard]] auto operator[](usize offset) const -> const byte&;

    // --- Iterators
    [[nodiscard]] auto begin() -> byte*;
    [[nodiscard]] auto begin() const -> const byte*;
    [[nodiscard]] auto cbegin() -> byte*;
    [[nodiscard]] auto cbegin() const -> const byte*;

    [[nodiscard]] auto end() -> byte*;
    [[nodiscard]] auto end() const -> const byte*;
    [[nodiscard]] auto cend() -> byte*;
    [[nodiscard]] auto cend() const -> const byte*;

    // --- Size and capacity
    [[nodiscard]] auto empty() const { return _size_bytes == 0; }
    [[nodiscard]] auto size() const { return _size_bytes; }
    [[nodiscard]] auto size_bytes() const { return _size_bytes; }
    [[nodiscard]] auto capacity() const { return _size_bytes; }

    auto resize(usize size_bytes) -> void;
    auto free() -> void;

    // --- Modifiers
    // Returns the number of bytes written.
    auto buffer_data(const void* data, usize data_size_bytes, usize offset = 0) -> usize;
    // Returns the number of bytes written.
    auto buffer_data(auto&& data, usize offset = 0) -> usize;
    // Returns the number of bytes written.
    auto buffer_data(std::ranges::contiguous_range auto&& data, usize offset = 0) -> usize;

private:
    usize _size_bytes = 0;
    byte* _data = nullptr; // We assume that _data is always properly aligned to point to any type.
    // We're not using unique_ptr because a unique_ptr makes it more messy to reallocate the data.

private:
    auto allocate(usize size_bytes) -> void;
    auto reallocate(usize size_bytes) -> void;
};

class DynamicBuffer
{
public:
    using ByteRef = std::reference_wrapper<byte>;
    using ConstByteRef = std::reference_wrapper<const byte>;

    explicit DynamicBuffer() = default;
    explicit DynamicBuffer(usize size_bytes);
    explicit DynamicBuffer(const void* data, usize data_size_bytes);
    explicit DynamicBuffer(std::ranges::contiguous_range auto&& data);

    [[nodiscard]] static auto with_size(usize size_bytes) -> DynamicBuffer;
    [[nodiscard]] static auto with_data(const void* data, usize data_size_bytes) -> DynamicBuffer;
    [[nodiscard]] static auto with_data(auto&& data) -> DynamicBuffer;
    [[nodiscard]] static auto with_data(std::ranges::contiguous_range auto&& data) -> DynamicBuffer;

    DynamicBuffer(const DynamicBuffer& other);
    auto operator=(const DynamicBuffer& other) -> DynamicBuffer&;

    DynamicBuffer(DynamicBuffer&& other) noexcept;
    auto operator=(DynamicBuffer&& other) noexcept -> DynamicBuffer&;

    ~DynamicBuffer();

    // --- Data access
    [[nodiscard]] auto data() & { return _data; }
    [[nodiscard]] auto data() const& -> const auto* { return _data; }
    [[nodiscard]] auto data() && -> byte*; // The returned pointer must be freed!

    [[nodiscard]] auto at(usize offset) -> std::optional<ByteRef>;
    [[nodiscard]] auto at(usize offset) const -> std::optional<ConstByteRef>;

    [[nodiscard]] auto operator[](usize offset) -> byte&;
    [[nodiscard]] auto operator[](usize offset) const -> const byte&;

    // --- Iterators
    [[nodiscard]] auto begin() -> byte*;
    [[nodiscard]] auto begin() const -> const byte*;
    [[nodiscard]] auto cbegin() -> byte*;
    [[nodiscard]] auto cbegin() const -> const byte*;

    [[nodiscard]] auto end() -> byte*;
    [[nodiscard]] auto end() const -> const byte*;
    [[nodiscard]] auto cend() -> byte*;
    [[nodiscard]] auto cend() const -> const byte*;

    // --- Size and capacity
    [[nodiscard]] auto empty() const { return _size_bytes == 0; }
    [[nodiscard]] auto size() const { return _size_bytes; }
    [[nodiscard]] auto size_bytes() const { return _size_bytes; }
    [[nodiscard]] auto capacity() const { return _capacity_bytes; }

    auto resize(usize size_bytes) -> void;
    auto reserve(usize min_capacity_bytes) -> void;
    auto shrink_to_fit() -> void;
    auto free() -> void;

    // --- Modifiers
    // Returns the number of bytes written.
    auto buffer_data(const void* data, usize data_size_bytes, usize offset = 0) -> usize;
    // Returns the number of bytes written.
    auto buffer_data(auto&& data, usize offset = 0) -> usize;
    // Returns the number of bytes written.
    auto buffer_data(std::ranges::contiguous_range auto&& data, usize offset = 0) -> usize;

    auto clear() -> void;

private:
    usize _size_bytes = 0;
    usize _capacity_bytes = 0;
    byte* _data = nullptr; // We assume that _data is always properly aligned to point to any type.
    // We're not using unique_ptr because a unique_ptr makes it more messy to reallocate the data.

private:
    auto resize_to_at_least(usize min_size_bytes) -> void;

    auto allocate(usize capacity_bytes) -> void;
    auto reallocate_exactly(usize new_capacity_bytes) -> void;
    auto reallocate_at_least(usize min_capacity_bytes) -> void;

    [[nodiscard]] static auto calculate_growth(usize old_size_bytes) -> usize;
};

} // namespace zth::memory

#include "buffer.inl"
