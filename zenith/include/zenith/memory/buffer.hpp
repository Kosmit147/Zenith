#pragma once

#include <array>
#include <ranges>

#include "zenith/core/typedefs.hpp"
#include "zenith/memory/memory.hpp"

namespace zth {

template<usize Size, usize Alignment = default_alignment> class alignas(Alignment) StaticBuffer
{
public:
    alignas(Alignment) std::array<byte, Size> data;

    // @todo
};

class Buffer
{
public:
    explicit Buffer() = default;
    explicit Buffer(usize size);
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
    [[nodiscard]] auto data() && -> byte*; // the returned pointer must be freed!

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
    [[nodiscard]] auto empty() const { return _size == 0; }
    [[nodiscard]] auto size() const { return _size; }
    [[nodiscard]] auto size_bytes() const { return _size; }
    [[nodiscard]] auto capacity() const { return _size; }

    auto resize(usize size_bytes) -> void;
    auto free() -> void;

    // --- Modifiers
    // returns the number of bytes written
    auto buffer_data(const void* data, usize data_size_bytes, usize offset = 0) -> usize;
    // returns the number of bytes written
    auto buffer_data(auto&& data, usize offset = 0) -> usize;
    // returns the number of bytes written
    auto buffer_data(std::ranges::contiguous_range auto&& data, usize offset = 0) -> usize;

private:
    usize _size = 0;
    byte* _data = nullptr; // since we're allocating with malloc, _data is always properly aligned to point to any type

private:
    auto allocate(usize size_bytes) -> void;
    auto reallocate(usize size_bytes) -> void;
};

class DynamicBuffer
{
public:
    static constexpr usize growth_factor = 2;

public:
    explicit DynamicBuffer() = default;
    explicit DynamicBuffer(usize size);
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
    [[nodiscard]] auto data() && -> byte*; // the returned pointer must be freed!

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
    [[nodiscard]] auto empty() const { return _size == 0; }
    [[nodiscard]] auto size() const { return _size; }
    [[nodiscard]] auto size_bytes() const { return _size; }
    [[nodiscard]] auto capacity() const { return _capacity; }

    auto resize(usize size_bytes) -> void;
    auto reserve(usize min_capacity_bytes) -> void;
    auto shrink_to_fit() -> void;
    auto free() -> void;

    // --- Modifiers
    // returns the number of bytes written
    auto buffer_data(const void* data, usize data_size_bytes, usize offset = 0) -> usize;
    // returns the number of bytes written
    auto buffer_data(auto&& data, usize offset = 0) -> usize;
    // returns the number of bytes written
    auto buffer_data(std::ranges::contiguous_range auto&& data, usize offset = 0) -> usize;

    auto clear() -> void;

private:
    usize _size = 0;
    usize _capacity = 0;
    byte* _data = nullptr; // since we're allocating with malloc, _data is always properly aligned to point to any type

private:
    auto resize_to_at_least(usize min_size_bytes) -> void;

    auto allocate(usize capacity_bytes) -> void;
    auto reallocate_exactly(usize capacity_bytes) -> void;
    auto reallocate_at_least(usize min_capacity_bytes) -> void;
};

} // namespace zth

#include "buffer.inl"
