#include <cstdint>
#include <limits>
#include <type_traits>

#include <zenith/meta/type.hpp>

static_assert(std::is_same_v<zth::meta::MinimalUint<std::numeric_limits<std::uint8_t>::min()>, std::uint8_t>);
static_assert(std::is_same_v<zth::meta::MinimalUint<std::numeric_limits<std::uint8_t>::max()>, std::uint8_t>);

static_assert(std::is_same_v<zth::meta::MinimalUint<std::numeric_limits<std::uint8_t>::max() + 1u>, std::uint16_t>);
static_assert(std::is_same_v<zth::meta::MinimalUint<std::numeric_limits<std::uint16_t>::max()>, std::uint16_t>);

static_assert(std::is_same_v<zth::meta::MinimalUint<std::numeric_limits<std::uint16_t>::max() + 1u>, std::uint32_t>);
static_assert(std::is_same_v<zth::meta::MinimalUint<std::numeric_limits<std::uint32_t>::max()>, std::uint32_t>);

static_assert(std::is_same_v<zth::meta::MinimalUint<std::numeric_limits<std::uint32_t>::max() + 1uLL>, std::uint64_t>);
static_assert(std::is_same_v<zth::meta::MinimalUint<std::numeric_limits<std::uint64_t>::max()>, std::uint64_t>);
