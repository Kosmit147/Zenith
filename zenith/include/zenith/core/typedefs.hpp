#pragma once

#include <cstddef>
#include <cstdint>

namespace zth {

using u8 = std::uint8_t;
using i8 = std::int8_t;
using u16 = std::uint16_t;
using i16 = std::int16_t;
using u32 = std::uint32_t;
using i32 = std::int32_t;
using u64 = std::uint64_t;
using i64 = std::int64_t;
using isize = std::intmax_t;
using usize = std::uintmax_t;

// @refactor: Use types from <stdfloat> when they become available.
using f32 = float;
using f64 = double;

static_assert(sizeof(f32) == 4);
static_assert(sizeof(f64) == 8);

using byte = std::byte;

using iptr_t = std::intptr_t;
using uptr_t = std::uintptr_t;

} // namespace zth
