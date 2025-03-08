#pragma once

#include <cstddef>

#include "zenith/core/typedefs.hpp"

namespace zth::memory {

constexpr usize minimal_alignment = alignof(std::max_align_t);
constexpr usize default_alignment = 16; // @speed: maybe default alignment should be minimal_alignment instead of 16

template<typename T> [[nodiscard]] constexpr auto is_aligned(T* ptr, usize alignment = alignof(T)) -> bool;

template<typename T> [[nodiscard]] constexpr auto aligned(T* ptr, usize alignment = alignof(T)) -> T*;
template<typename T> [[nodiscard]] constexpr auto aligned_down(T* ptr, usize alignment = alignof(T)) -> T*;

// returns the number of bytes ptr was shifted by
template<typename T> constexpr auto align(T*& ptr, usize alignment = alignof(T)) -> usize;
// returns the number of bytes ptr was shifted by
template<typename T> constexpr auto align_down(T*& ptr, usize alignment = alignof(T)) -> usize;

[[nodiscard]] constexpr auto kilobytes(usize n) -> usize;
[[nodiscard]] constexpr auto megabytes(usize n) -> usize;
[[nodiscard]] constexpr auto gigabytes(usize n) -> usize;

namespace size_literals {

[[nodiscard]] constexpr auto operator""_kb(usize n) -> usize;
[[nodiscard]] constexpr auto operator""_mb(usize n) -> usize;
[[nodiscard]] constexpr auto operator""_gb(usize n) -> usize;

} // namespace memory_size_literals

} // namespace zth::memory

#include "memory.inl"
