#include <zenith/core/typedefs.hpp>
#include <zenith/util/number.hpp>

static_assert(zth::highest_bit_set(static_cast<zth::u8>(0b10000000)));
static_assert(zth::highest_bit_set(static_cast<zth::u8>(0b11000110)));
static_assert(!zth::highest_bit_set(static_cast<zth::u8>(0b01000110)));
static_assert(!zth::highest_bit_set(static_cast<zth::u8>(0b00000000)));
