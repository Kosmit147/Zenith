#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "zenith/core/typedefs.hpp"
#include "zenith/memory/buffer.hpp"
#include "zenith/memory/memory.hpp"

namespace zth {

class TemporaryStorage
{
public:
    static constexpr usize initial_capacity = memory::megabytes(10);

public:
    TemporaryStorage() = delete;

    static auto init() -> void;
    static auto on_update() -> void;
    static auto shut_down() -> void;

    static auto reset() -> void;
    static auto reset_with_new_capacity(usize new_capacity) -> void;

    [[nodiscard]] static auto allocate(usize size_bytes, usize alignment = memory::default_alignment) -> void*;
    [[nodiscard]] static auto allocate_unaligned(usize size_bytes) -> void*;

    [[nodiscard]] static auto capacity() { return _buffer.size(); }
    [[nodiscard]] static auto space_left() -> usize;
    [[nodiscard]] static auto space_used() -> usize;

    [[nodiscard]] static auto begin() -> byte* { return _buffer.cbegin(); }
    [[nodiscard]] static auto end() -> byte* { return _buffer.cend(); }

private:
    static memory::Buffer _buffer;
    static inline byte* _buffer_ptr = nullptr;
    static std::vector<void*> _overflow_allocations;

private:
    [[nodiscard]] static auto allocate_if_overflowed(usize size_bytes) -> void*;
    static auto free_overflow_allocations() -> void;
};

template<typename T> struct TemporaryStorageAllocator
{
    // This is a minimal allocator implementation. Because of that, the standard library will automatically provide
    // efficient construct() and destruct() methods

    using value_type = T;

    TemporaryStorageAllocator() noexcept = default;
    template<typename U> TemporaryStorageAllocator(const TemporaryStorageAllocator<U>&) noexcept {}
    template<typename U> auto operator==(const TemporaryStorageAllocator<U>&) const noexcept -> bool { return true; }
    template<typename U> auto operator!=(const TemporaryStorageAllocator<U>&) const noexcept -> bool { return false; }

    auto allocate(std::size_t count) const -> T*;
    auto deallocate([[maybe_unused]] T* ptr, [[maybe_unused]] std::size_t count) const noexcept -> void {}
};

template<typename T> auto TemporaryStorageAllocator<T>::allocate(std::size_t count) const -> T*
{
    return static_cast<T*>(TemporaryStorage::allocate(count * sizeof(T), alignof(T)));
}

using TemporaryString = std::basic_string<char, std::char_traits<char>, TemporaryStorageAllocator<char>>;

} // namespace zth
