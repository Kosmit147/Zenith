#pragma once

#include <cstddef>
#include <memory>
#include <ranges>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "zenith/core/typedefs.hpp"
#include "zenith/memory/buffer.hpp"
#include "zenith/memory/memory.hpp"

namespace zth {

// A linear memory allocator which gets reset every frame. If we exceed the capacity, we resort to a regular allocation
// using new.
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
    static std::vector<std::unique_ptr<byte[]>> _overflow_allocations;

private:
    [[nodiscard]] static auto allocate_if_overflowed(usize size_bytes) -> void*;
    static auto free_overflow_allocations() -> void;
};

template<typename T> struct TemporaryStorageAllocator
{
    // A minimal allocator implementation.

    using value_type = T;

    TemporaryStorageAllocator() noexcept = default;
    template<typename U> TemporaryStorageAllocator(const TemporaryStorageAllocator<U>&) noexcept {}
    template<typename U> auto operator==(const TemporaryStorageAllocator<U>&) const noexcept -> bool { return true; }
    template<typename U> auto operator!=(const TemporaryStorageAllocator<U>&) const noexcept -> bool { return false; }

    auto allocate(std::size_t count) const noexcept -> T*;
    auto deallocate(T* ptr, std::size_t count) const noexcept -> void;
};

template<typename T> using Temporary = std::unique_ptr<T, memory::DestroyingDeleter<T>>;
using TemporaryString = std::basic_string<char, std::char_traits<char>, TemporaryStorageAllocator<char>>;
template<typename T> using TemporaryVector = std::vector<T, TemporaryStorageAllocator<T>>;

// @cleanup: These make_temporary functions could probably be generalized and could take in an allocator with which to
// construct the object as a template parameter.
// @todo: Implement make_temporary_for_overwrite (analogous to std::make_unique_for_overwrite).

template<typename T, typename... Args>
[[nodiscard]] auto make_temporary(Args&&... args) -> Temporary<T>
    requires(!std::is_array_v<T>)
{
    auto ptr = static_cast<T*>(TemporaryStorage::allocate(sizeof(T), alignof(T)));
    std::construct_at(ptr, std::forward<decltype(args)>(args)...);
    return Temporary<T>{ ptr };
}

// Specialization for array types.
template<typename T>
[[nodiscard]] auto make_temporary() -> Temporary<T>
    requires(std::is_bounded_array_v<T>)
{
    auto ptr = static_cast<T*>(TemporaryStorage::allocate(sizeof(T), alignof(T)));
    std::ranges::uninitialized_default_construct(*ptr);
    return Temporary<T>{ ptr };
}

// Specialization for array types.
template<typename T, typename... Args>
[[nodiscard]] auto make_temporary(Args&&... args) -> Temporary<T>
    requires(std::is_bounded_array_v<T>)
{
    auto ptr = static_cast<T*>(TemporaryStorage::allocate(sizeof(T), alignof(T)));

    for (auto& elem : *ptr)
        std::construct_at(std::addressof(elem), std::forward<decltype(args)>(args)...);

    return Temporary<T>{ ptr };
}

// We don't have a Temporary pointing to a T[], like a unique_ptr can, because then we don't know how many Ts we have to
// destruct. If we wanted to support that, we would have to store additional metadata containing information about the
// number of constructed Ts.
// @todo: Handle arrays of dynamic size (T[]). We should take into account whether the type is trivially destructible,
// because then we don't have to actually store the additional metadata.
template<typename T, typename... Args>
[[nodiscard]] auto make_temporary(Args&&...) -> Temporary<T>
    requires(std::is_unbounded_array_v<T>)
= delete;

template<typename T> auto TemporaryStorageAllocator<T>::allocate(std::size_t count) const noexcept -> T*
{
    return static_cast<T*>(TemporaryStorage::allocate(count * sizeof(T), alignof(T)));
}

template<typename T>
auto TemporaryStorageAllocator<T>::deallocate([[maybe_unused]] T* ptr,
                                              [[maybe_unused]] std::size_t count) const noexcept -> void
{}

} // namespace zth
