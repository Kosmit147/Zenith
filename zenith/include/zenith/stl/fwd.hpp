#pragma once

#include <concepts>

#include "zenith/core/typedefs.hpp"

namespace zth {

class ContiguousRangeInterface;

namespace string {

struct FindResult;

} // namespace string

struct StringHasher;

template<std::movable T, usize Capacity> class InPlaceVector;

} // namespace zth
