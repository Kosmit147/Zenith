#pragma once

#include <memory>
#include <ranges>

#include "zenith/core/assert.hpp"
#include "zenith/math/number.hpp"

namespace zth::memory {

// @refactor: Use std::is_sufficiently_aligned once we're on C++26.
template<typename T> constexpr auto is_aligned(T* ptr, usize alignment) -> bool
{
    ZTH_ASSERT(math::is_power_of_2(alignment));
    auto address = reinterpret_cast<uptr_t>(ptr);
    auto mask = alignment - 1;
    return !(address & mask);
}

template<typename T> constexpr auto aligned(T* ptr, usize alignment) -> T*
{
    ZTH_ASSERT(math::is_power_of_2(alignment));
    auto address = reinterpret_cast<uptr_t>(ptr);
    address += (alignment - 1);
    return aligned_down(reinterpret_cast<T*>(address), alignment);
}

template<typename T> constexpr auto aligned_down(T* ptr, usize alignment) -> T*
{
    ZTH_ASSERT(math::is_power_of_2(alignment));
    auto address = reinterpret_cast<uptr_t>(ptr);
    auto mask = alignment - 1;
    return reinterpret_cast<T*>(address & ~mask);
}

template<typename T> constexpr auto align(T*& ptr, usize alignment) -> usize
{
    ZTH_ASSERT(math::is_power_of_2(alignment));
    auto address = reinterpret_cast<uptr_t>(ptr);
    auto mask = alignment - 1;
    auto offset = address & mask;

    if (offset != 0)
        offset = alignment - offset;

    ptr = reinterpret_cast<T*>(address + offset);
    return offset;
}

template<typename T> constexpr auto align_down(T*& ptr, usize alignment) -> usize
{
    ZTH_ASSERT(math::is_power_of_2(alignment));
    auto address = reinterpret_cast<uptr_t>(ptr);
    auto mask = alignment - 1;
    auto offset = address & mask;
    ptr = reinterpret_cast<T*>(address - offset);
    return offset;
}

constexpr auto kilobytes(usize n) -> usize
{
    return n * 1024u;
}

constexpr auto megabytes(usize n) -> usize
{
    return kilobytes(n) * 1024u;
}

constexpr auto gigabytes(usize n) -> usize
{
    return megabytes(n) * 1024u;
}

namespace size_literals {

constexpr auto operator""_kb(usize n) -> usize
{
    return kilobytes(n);
}

constexpr auto operator""_mb(usize n) -> usize
{
    return megabytes(n);
}

constexpr auto operator""_gb(usize n) -> usize
{
    return gigabytes(n);
}

} // namespace size_literals

template<std::destructible T>
    requires(!std::is_unbounded_array_v<T>)
constexpr auto DestroyingDeleter<T>::operator()(T* ptr) const noexcept -> void
{
    std::ranges::destroy_at(ptr);
}

} // namespace zth::memory
