#pragma once

#include <concepts>
#include <memory>
#include <type_traits>

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

// Destructs an object and deallocates the memory using our custom allocation functions.
// Handles pointers to arrays, but not arrays of dynamic size (T[]).
// @todo: Handle arrays of dynamic size (T[]). We should take into account whether the type is trivially destructible,
// because then we don't have to actually store the additional metadata nor call any destructors
template<std::destructible T>
    requires(!std::is_unbounded_array_v<T>)
struct CustomDeleter
{
public:
    constexpr CustomDeleter() noexcept = default;

    template<typename U>
    constexpr CustomDeleter(const CustomDeleter<U>&) noexcept
        requires(std::convertible_to<U*, T*>)
    {}

    // std::destructible ensures that T's destructor doesn't throw
    constexpr auto operator()(T* ptr) const noexcept -> void;
};

template<typename T> struct CustomAllocator
{
    // A minimal allocator implementation.

    using value_type = T;

    CustomAllocator() noexcept = default;
    template<typename U> CustomAllocator(const CustomAllocator<U>&) noexcept {}
    template<typename U> auto operator==(const CustomAllocator<U>&) const noexcept -> bool { return true; }
    template<typename U> auto operator!=(const CustomAllocator<U>&) const noexcept -> bool { return false; }

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

template<std::destructible T>
    requires(!std::is_unbounded_array_v<T>)
constexpr auto CustomDeleter<T>::operator()(T* ptr) const noexcept -> void
{
    std::destroy_at(ptr);
    deallocate(ptr);
}

template<typename T> auto CustomAllocator<T>::allocate(std::size_t count) const noexcept -> T*
{
    return static_cast<T*>(memory::allocate(count * sizeof(T)));
}

template<typename T>
auto CustomAllocator<T>::deallocate(T* ptr, [[maybe_unused]] std::size_t count) const noexcept -> void
{
    memory::deallocate(ptr);
}

} // namespace zth::memory
