#pragma once

#include <concepts>

#include "zenith/core/typedefs.hpp"

namespace zth {

template<std::movable T, usize Capacity> class InPlaceVector;

namespace string {

struct FindResult;

} // namespace string

struct StringHasher;

} // namespace zth
