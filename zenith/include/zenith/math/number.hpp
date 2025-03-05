#pragma once

#include <bit>
#include <concepts>

namespace zth {

[[nodiscard]] constexpr auto is_power_of_2(std::unsigned_integral auto n) -> bool
{
    return std::has_single_bit(n);
}

} // namespace zth
