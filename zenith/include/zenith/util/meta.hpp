#pragma once

#include <limits>
#include <type_traits>

#include "zenith/core/typedefs.hpp"

namespace zth {

// clang-format off

// Smallest unsigned integer that can represent values in range <0, N>.
template<usize N> using SmallestUintContaining = 
    std::conditional_t<N <= std::numeric_limits<u8>::max(), u8,
    std::conditional_t<N <= std::numeric_limits<u16>::max(), u16,
    std::conditional_t<N <= std::numeric_limits<u32>::max(), u32,
    std::conditional_t<N <= std::numeric_limits<u64>::max(), u64,
    usize>>>>;

// clang-format on

} // namespace zth
