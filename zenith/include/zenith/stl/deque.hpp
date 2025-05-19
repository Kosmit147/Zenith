#pragma once

#include <concepts>
#include <deque>

#include "zenith/memory/alloc.hpp"

namespace zth {

template<std::movable T, memory::Allocator A = memory::DefaultAllocator<T>> using Deque = std::deque<T, A>;

} // namespace zth
