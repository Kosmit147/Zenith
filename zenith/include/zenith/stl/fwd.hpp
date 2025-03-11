#pragma once

#include <concepts>

#include "zenith/core/typedefs.hpp"

namespace zth {

template<std::movable T, usize Capacity> class InPlaceVector;

struct StringHasher;

} // namespace zth
