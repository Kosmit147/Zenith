#pragma once

#include <concepts>
#include <type_traits>

#include "zenith/core/typedefs.hpp"
#include "zenith/memory/memory.hpp"

namespace zth {

namespace memory {

template<std::destructible T>
    requires(!std::is_unbounded_array_v<T>)
struct CustomDeleter;
template<typename T> struct CustomAllocator;

template<usize Size, usize Alignment = default_alignment> class alignas(Alignment) StaticBuffer;
class Buffer;
class DynamicBuffer;

template<std::destructible T>
    requires(!std::is_unbounded_array_v<T>)
struct DestroyingDeleter;

} // namespace memory

class TemporaryStorage;
template<typename T> struct TemporaryStorageAllocator;

} // namespace zth
