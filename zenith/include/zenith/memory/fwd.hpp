#pragma once

#include "zenith/core/typedefs.hpp"
#include "zenith/memory/memory.hpp"

namespace zth {

namespace memory {

template<usize Size, usize Alignment = default_alignment> class alignas(Alignment) StaticBuffer;
class Buffer;
class DynamicBuffer;

template<typename T> struct Allocator;

} // namespace memory

class TemporaryStorage;
template<typename T> struct TemporaryStorageAllocator;

} // namespace zth
