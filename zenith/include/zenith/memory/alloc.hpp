#pragma once

#include "zenith/core/typedefs.hpp"

namespace zth::memory {

[[nodiscard]] auto allocate(usize size_bytes) noexcept -> void*;

// modifies ptr to point to the new location
template<typename T> auto reallocate(T*& ptr, usize new_size_bytes) noexcept -> void;
// modifies ptr to point to the new location
template<> auto reallocate(void*& ptr, usize new_size_bytes) noexcept -> void;

// modifies ptr to point to the new location. buffer_size_bytes determines how many bytes will be copied to the new
// destination
template<typename T> auto reallocate(T*& ptr, usize new_size_bytes, usize buffer_size_bytes) noexcept -> void;
// modifies ptr to point to the new location. buffer_size_bytes determines how many bytes will be copied to the new
// destination
template<> auto reallocate(void*& ptr, usize new_size_bytes, usize buffer_size_bytes) noexcept -> void;

// this function sets ptr to nullptr
template<typename T> auto deallocate(T*& ptr) noexcept -> void;
// this function sets ptr to nullptr
template<> auto deallocate(void*& ptr) noexcept -> void;

template<typename T> struct Allocator
{
    // A minimal allocator implementation.

    using value_type = T;

    Allocator() noexcept = default;
    template<typename U> Allocator(const Allocator<U>&) noexcept {}
    template<typename U> auto operator==(const Allocator<U>&) const noexcept -> bool { return true; }
    template<typename U> auto operator!=(const Allocator<U>&) const noexcept -> bool { return false; }

    auto allocate(std::size_t count) const noexcept -> T*;
    auto deallocate(T* ptr, std::size_t count) const noexcept -> void;
};

template<typename T> auto reallocate(T*& ptr, usize new_size_bytes) noexcept -> void
{
    reallocate(reinterpret_cast<void*&>(ptr), new_size_bytes);
}

template<typename T> auto reallocate(T*& ptr, usize new_size_bytes, usize buffer_size_bytes) noexcept -> void
{
    reallocate(reinterpret_cast<void*&>(ptr), new_size_bytes, buffer_size_bytes);
}

template<typename T> auto deallocate(T*& ptr) noexcept -> void
{
    deallocate(reinterpret_cast<void*&>(ptr));
}

template<typename T> auto Allocator<T>::allocate(std::size_t count) const noexcept -> T*
{
    return static_cast<T*>(memory::allocate(count * sizeof(T)));
}

template<typename T> auto Allocator<T>::deallocate(T* ptr, [[maybe_unused]] std::size_t count) const noexcept -> void
{
    memory::deallocate(ptr);
}

} // namespace zth::memory
