#pragma once

#include <concepts>

#include "zenith/core/typedefs.hpp"

namespace zth {

class ContiguousRangeInterface;

struct StringHasher;

template<std::movable T, usize Capacity> class InPlaceVector;

} // namespace zth
