#pragma once

#include <cstddef>
#include <span>

namespace zth {

// Converts a span with a static extent to a span with a dynamic extent. Leaves spans with dynamic extents unchanged.
template<typename T, std::size_t Extent>
[[nodiscard]] constexpr auto make_dynamic_span(std::span<T, Extent> span) -> std::span<T, std::dynamic_extent>
{
    return span;
}

} // namespace zth
