#include <zenith/core/cast.hpp>
#include <zenith/core/typedefs.hpp>

// --------------------------- can_cast ---------------------------

// Same sizes.

// u8 -> i8
static_assert(zth::can_cast<zth::i8>(static_cast<zth::u8>(0)));
static_assert(zth::can_cast<zth::i8>(static_cast<zth::u8>(127)));
static_assert(!zth::can_cast<zth::i8>(static_cast<zth::u8>(128)));
static_assert(!zth::can_cast<zth::i8>(static_cast<zth::u8>(255)));

// i8 -> u8
static_assert(!zth::can_cast<zth::u8>(static_cast<zth::i8>(-128)));
static_assert(!zth::can_cast<zth::u8>(static_cast<zth::i8>(-1)));
static_assert(zth::can_cast<zth::u8>(static_cast<zth::i8>(0)));
static_assert(zth::can_cast<zth::u8>(static_cast<zth::i8>(127)));

// Smaller into bigger.

// i8 -> i16
static_assert(zth::can_cast<zth::i16>(static_cast<zth::i8>(-128)));
static_assert(zth::can_cast<zth::i16>(static_cast<zth::i8>(-1)));
static_assert(zth::can_cast<zth::i16>(static_cast<zth::i8>(0)));
static_assert(zth::can_cast<zth::i16>(static_cast<zth::i8>(127)));

// i8 -> u16
static_assert(!zth::can_cast<zth::u16>(static_cast<zth::i8>(-128)));
static_assert(!zth::can_cast<zth::u16>(static_cast<zth::i8>(-1)));
static_assert(zth::can_cast<zth::u16>(static_cast<zth::i8>(0)));
static_assert(zth::can_cast<zth::u16>(static_cast<zth::i8>(127)));

// u8 -> i16
static_assert(zth::can_cast<zth::i16>(static_cast<zth::u8>(0)));
static_assert(zth::can_cast<zth::i16>(static_cast<zth::u8>(127)));
static_assert(zth::can_cast<zth::i16>(static_cast<zth::u8>(128)));
static_assert(zth::can_cast<zth::i16>(static_cast<zth::u8>(255)));

// u8 -> u16
static_assert(zth::can_cast<zth::u16>(static_cast<zth::u8>(0)));
static_assert(zth::can_cast<zth::u16>(static_cast<zth::u8>(127)));
static_assert(zth::can_cast<zth::u16>(static_cast<zth::u8>(128)));
static_assert(zth::can_cast<zth::u16>(static_cast<zth::u8>(255)));

// Bigger into smaller.

// i16 -> i8
static_assert(!zth::can_cast<zth::i8>(static_cast<zth::i16>(-32768)));
static_assert(zth::can_cast<zth::i8>(static_cast<zth::i16>(-1)));
static_assert(zth::can_cast<zth::i8>(static_cast<zth::i16>(0)));
static_assert(!zth::can_cast<zth::i8>(static_cast<zth::i16>(32767)));

// i16 -> u8
static_assert(!zth::can_cast<zth::u8>(static_cast<zth::i16>(-32768)));
static_assert(!zth::can_cast<zth::u8>(static_cast<zth::i16>(-1)));
static_assert(zth::can_cast<zth::u8>(static_cast<zth::i16>(0)));
static_assert(!zth::can_cast<zth::u8>(static_cast<zth::i16>(32767)));

// u16 -> i8
static_assert(zth::can_cast<zth::i8>(static_cast<zth::u16>(0)));
static_assert(!zth::can_cast<zth::i8>(static_cast<zth::u16>(32767)));
static_assert(!zth::can_cast<zth::i8>(static_cast<zth::u16>(32768)));
static_assert(!zth::can_cast<zth::i8>(static_cast<zth::u16>(65535)));

// u16 -> u8
static_assert(zth::can_cast<zth::u8>(static_cast<zth::u16>(0)));
static_assert(!zth::can_cast<zth::u8>(static_cast<zth::u16>(32767)));
static_assert(!zth::can_cast<zth::u8>(static_cast<zth::u16>(32768)));
static_assert(!zth::can_cast<zth::u8>(static_cast<zth::u16>(65535)));

// --------------------------- clamping_cast ---------------------------

// Same sizes.

// u8 -> i8
static_assert(zth::clamping_cast<zth::i8>(static_cast<zth::u8>(0)) == 0);
static_assert(zth::clamping_cast<zth::i8>(static_cast<zth::u8>(127)) == 127);
static_assert(zth::clamping_cast<zth::i8>(static_cast<zth::u8>(128)) == 127);
static_assert(zth::clamping_cast<zth::i8>(static_cast<zth::u8>(255)) == 127);

// i8 -> u8
static_assert(zth::clamping_cast<zth::u8>(static_cast<zth::i8>(-128)) == 0);
static_assert(zth::clamping_cast<zth::u8>(static_cast<zth::i8>(-1)) == 0);
static_assert(zth::clamping_cast<zth::u8>(static_cast<zth::i8>(0)) == 0);
static_assert(zth::clamping_cast<zth::u8>(static_cast<zth::i8>(127)) == 127);

// Smaller into bigger.

// i8 -> i16
static_assert(zth::clamping_cast<zth::i16>(static_cast<zth::i8>(-128)) == -128);
static_assert(zth::clamping_cast<zth::i16>(static_cast<zth::i8>(-1)) == -1);
static_assert(zth::clamping_cast<zth::i16>(static_cast<zth::i8>(0)) == 0);
static_assert(zth::clamping_cast<zth::i16>(static_cast<zth::i8>(127)) == 127);

// i8 -> u16
static_assert(zth::clamping_cast<zth::u16>(static_cast<zth::i8>(-128)) == 0);
static_assert(zth::clamping_cast<zth::u16>(static_cast<zth::i8>(-1)) == 0);
static_assert(zth::clamping_cast<zth::u16>(static_cast<zth::i8>(0)) == 0);
static_assert(zth::clamping_cast<zth::u16>(static_cast<zth::i8>(127)) == 127);

// u8 -> i16
static_assert(zth::clamping_cast<zth::i16>(static_cast<zth::u8>(0)) == 0);
static_assert(zth::clamping_cast<zth::i16>(static_cast<zth::u8>(127)) == 127);
static_assert(zth::clamping_cast<zth::i16>(static_cast<zth::u8>(128)) == 128);
static_assert(zth::clamping_cast<zth::i16>(static_cast<zth::u8>(255)) == 255);

// u8 -> u16
static_assert(zth::clamping_cast<zth::u16>(static_cast<zth::u8>(0)) == 0);
static_assert(zth::clamping_cast<zth::u16>(static_cast<zth::u8>(127)) == 127);
static_assert(zth::clamping_cast<zth::u16>(static_cast<zth::u8>(128)) == 128);
static_assert(zth::clamping_cast<zth::u16>(static_cast<zth::u8>(255)) == 255);

// Bigger into smaller.

// i16 -> i8
static_assert(zth::clamping_cast<zth::i8>(static_cast<zth::i16>(-32768)) == -128);
static_assert(zth::clamping_cast<zth::i8>(static_cast<zth::i16>(-1)) == -1);
static_assert(zth::clamping_cast<zth::i8>(static_cast<zth::i16>(0)) == 0);
static_assert(zth::clamping_cast<zth::i8>(static_cast<zth::i16>(32767)) == 127);

// i16 -> u8
static_assert(zth::clamping_cast<zth::u8>(static_cast<zth::i16>(-32768)) == 0);
static_assert(zth::clamping_cast<zth::u8>(static_cast<zth::i16>(-1)) == 0);
static_assert(zth::clamping_cast<zth::u8>(static_cast<zth::i16>(0)) == 0);
static_assert(zth::clamping_cast<zth::u8>(static_cast<zth::i16>(32767)) == 255);

// u16 -> i8
static_assert(zth::clamping_cast<zth::i8>(static_cast<zth::u16>(0)) == 0);
static_assert(zth::clamping_cast<zth::i8>(static_cast<zth::u16>(32767)) == 127);
static_assert(zth::clamping_cast<zth::i8>(static_cast<zth::u16>(32768)) == 127);
static_assert(zth::clamping_cast<zth::i8>(static_cast<zth::u16>(65535)) == 127);

// u16 -> u8
static_assert(zth::clamping_cast<zth::u8>(static_cast<zth::u16>(0)) == 0);
static_assert(zth::clamping_cast<zth::u8>(static_cast<zth::u16>(32767)) == 255);
static_assert(zth::clamping_cast<zth::u8>(static_cast<zth::u16>(32768)) == 255);
static_assert(zth::clamping_cast<zth::u8>(static_cast<zth::u16>(65535)) == 255);
