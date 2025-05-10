#pragma once

#include <bit>
#include <concepts>

namespace zth {

[[nodiscard]] constexpr auto is_power_of_2(std::unsigned_integral auto n) -> bool
{
    return std::has_single_bit(n);
}

template<std::integral I> [[nodiscard]] constexpr auto highest_bit_set(I n) -> bool
{
    I highest_bit = static_cast<I>(1) << (sizeof(n) * 8 - 1);
    return n & highest_bit;
}

} // namespace zth
