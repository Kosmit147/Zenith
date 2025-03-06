#pragma once

#include "zenith/core/typedefs.hpp"

namespace zth {

[[nodiscard]] auto allocate(usize size_bytes) noexcept -> void*;

template<typename T> auto reallocate(T*& ptr, usize new_size_bytes) noexcept -> void;
template<> auto reallocate(void*& ptr, usize new_size_bytes) noexcept -> void;

// buffer_size_bytes determines how many bytes will be copied to the new destination
template<typename T> auto reallocate(T*& ptr, usize new_size_bytes, usize buffer_size_bytes) noexcept -> void;
// buffer_size_bytes determines how many bytes will be copied to the new destination
template<> auto reallocate(void*& ptr, usize new_size_bytes, usize buffer_size_bytes) noexcept -> void;

auto free(void* ptr) noexcept -> void;

template<typename T> auto reallocate(T*& ptr, usize new_size_bytes) noexcept -> void
{
    reallocate(reinterpret_cast<void*&>(ptr), new_size_bytes);
}

template<typename T> auto reallocate(T*& ptr, usize new_size_bytes, usize buffer_size_bytes) noexcept -> void
{
    reallocate(reinterpret_cast<void*&>(ptr), new_size_bytes, buffer_size_bytes);
}

} // namespace zth
