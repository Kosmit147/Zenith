#pragma once

#include <concepts>
#include <deque>
#include <memory>

#include "zenith/memory/alloc.hpp"

namespace zth {

template<std::movable T, memory::Allocator A = std::allocator<T>> using Deque = std::deque<T, A>;

} // namespace zth
