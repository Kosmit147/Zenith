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
template<typename T> auto free(T*& ptr) noexcept -> void;
// this function sets ptr to nullptr
template<> auto free(void*& ptr) noexcept -> void;

template<typename T> auto reallocate(T*& ptr, usize new_size_bytes) noexcept -> void
{
    reallocate(reinterpret_cast<void*&>(ptr), new_size_bytes);
}

template<typename T> auto reallocate(T*& ptr, usize new_size_bytes, usize buffer_size_bytes) noexcept -> void
{
    reallocate(reinterpret_cast<void*&>(ptr), new_size_bytes, buffer_size_bytes);
}

template<typename T> auto free(T*& ptr) noexcept -> void
{
    free(reinterpret_cast<void*&>(ptr));
}

} // namespace zth::memory
