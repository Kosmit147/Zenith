#pragma once

#include <concepts>
#include <cstddef>
#include <type_traits>

#include "zenith/core/typedefs.hpp"

namespace zth::memory {

constexpr usize minimal_alignment = alignof(std::max_align_t);
constexpr usize default_alignment = 16; // @speed: Maybe default alignment should be minimal_alignment instead of 16.

template<typename T> [[nodiscard]] auto is_aligned(T* ptr, usize alignment = alignof(T)) -> bool;

template<typename T> [[nodiscard]] auto aligned(T* ptr, usize alignment = alignof(T)) -> T*;
template<typename T> [[nodiscard]] auto aligned_down(T* ptr, usize alignment = alignof(T)) -> T*;

// Returns the value ptr was incremented by.
template<typename T> auto align(T*& ptr, usize alignment = alignof(T)) -> usize;
// Returns the value ptr was decremented by.
template<typename T> auto align_down(T*& ptr, usize alignment = alignof(T)) -> usize;

[[nodiscard]] constexpr auto kilobytes(usize n) -> usize;
[[nodiscard]] constexpr auto megabytes(usize n) -> usize;
[[nodiscard]] constexpr auto gigabytes(usize n) -> usize;

namespace size_literals {

[[nodiscard]] constexpr auto operator""_kb(usize n) -> usize;
[[nodiscard]] constexpr auto operator""_mb(usize n) -> usize;
[[nodiscard]] constexpr auto operator""_gb(usize n) -> usize;

} // namespace size_literals

// Destructs an object without freeing the memory. Handles pointers to arrays, but not arrays of dynamic size (T[]).
// @todo: Handle arrays of dynamic size (T[]). We should take into account whether the type is trivially destructible,
// because then we don't have to actually store the additional metadata nor call any destructors. We could either store
// the information about the number of objects to destruct the same way the standard operator delete[] does, or store
// the number of allocated objects as part of the state of the deleter.
template<std::destructible T>
    requires(!std::is_unbounded_array_v<T>)
struct DestroyingDeleter
{
public:
    constexpr DestroyingDeleter() noexcept = default;

    template<typename U>
    constexpr DestroyingDeleter(const DestroyingDeleter<U>&) noexcept
        requires(std::convertible_to<U*, T*>)
    {}

    // std::destructible ensures that T's destructor doesn't throw.
    constexpr auto operator()(T* ptr) const noexcept -> void;
};

} // namespace zth::memory

#include "memory.inl"
