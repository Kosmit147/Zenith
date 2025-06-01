#pragma once

#include <cstddef>
#include <iterator>
#include <type_traits>
#include <utility>

#include "zenith/core/typedefs.hpp"
#include "zenith/log/format.hpp"
#include "zenith/memory/alloc.hpp"
#include "zenith/memory/buffer.hpp"
#include "zenith/memory/managed.hpp"
#include "zenith/memory/memory.hpp"
#include "zenith/stl/string.hpp"
#include "zenith/stl/vector.hpp"
#include "zenith/util/result.hpp"

namespace zth {

// A linear memory allocator which gets reset at the start of every frame. If we exceed the capacity, we resort to a
// regular allocation. The preferred way of interacting with temporary storage is via make_temporary().
class TemporaryStorage
{
public:
    TemporaryStorage() = delete;

    [[nodiscard]] static auto init(usize initial_capacity) -> Result<void, String>;
    static auto start_frame() -> void;
    static auto shut_down() -> void;

    static auto reset() -> void;
    static auto reset_with_new_capacity(usize new_capacity) -> void;

    [[nodiscard]] static auto allocate(usize size_bytes, usize alignment = memory::default_alignment) -> void*;
    [[nodiscard]] static auto allocate_unaligned(usize size_bytes) -> void*;

    [[nodiscard]] static auto capacity() -> usize;
    [[nodiscard]] static auto left() -> usize;
    [[nodiscard]] static auto used() -> usize;
    [[nodiscard]] static auto usage_last_frame() -> usize;

    [[nodiscard]] static auto begin() -> decltype(auto) { return _buffer.begin(); }
    [[nodiscard]] static auto end() -> decltype(auto) { return _buffer.end(); }

private:
    static memory::Buffer<> _buffer;
    static inline byte* _buffer_ptr = nullptr;
    static Vector<UniquePtr<byte[]>> _overflow_allocations;

    static inline usize _usage_last_frame = 0;

private:
    [[nodiscard]] static auto allocate_if_overflowed(usize size_bytes) -> void*;
    static auto free_overflow_allocations() -> void;
};

template<typename T> struct TemporaryStorageAllocator
{
    // A minimal allocator implementation.
    static_assert(!std::is_const_v<T>);
    static_assert(!std::is_function_v<T>);
    static_assert(!std::is_reference_v<T>);

    using value_type = T;

    TemporaryStorageAllocator() noexcept = default;
    template<typename U> TemporaryStorageAllocator(const TemporaryStorageAllocator<U>&) noexcept {}

    template<typename U> auto operator==(const TemporaryStorageAllocator<U>&) const noexcept -> bool { return true; }
    template<typename U> auto operator!=(const TemporaryStorageAllocator<U>&) const noexcept -> bool { return false; }

    auto allocate(std::size_t count) const noexcept -> T*;
    auto deallocate(T* ptr, std::size_t count) const noexcept -> void;
};

static_assert(memory::Allocator<TemporaryStorageAllocator<int>>);

// @cleanup: We should move these aliases somewhere else because they force us to include a lot of headers in this file.
template<typename T> using Temporary = UniquePtr<T, TemporaryStorageAllocator<std::remove_extent_t<T>>>;
using TemporaryString = GenericString<char, std::char_traits<char>, TemporaryStorageAllocator<char>>;
template<typename T> using TemporaryVector = Vector<T, TemporaryStorageAllocator<T>>;

using TemporaryBuffer = memory::Buffer<TemporaryStorageAllocator<byte>>;
using TemporaryDynamicBuffer = memory::DynamicBuffer<TemporaryStorageAllocator<byte>>;

template<typename T>
    requires(!std::is_unbounded_array_v<T>)
[[nodiscard]] auto make_temporary(auto&&... args) -> Temporary<T>
{
    return make_unique<T, TemporaryStorageAllocator<T>>(std::forward<decltype(args)>(args)...);
}

template<typename T>
    requires(std::is_unbounded_array_v<T>)
[[nodiscard]] auto make_temporary(usize count) -> Temporary<T>
{
    return make_unique<T, TemporaryStorageAllocator<std::remove_extent_t<T>>>(count);
}

template<typename T>
    requires(!std::is_unbounded_array_v<T>)
[[nodiscard]] auto make_temporary_for_overwrite(auto&&... args) -> Temporary<T>
{
    return make_unique_for_overwrite<T, TemporaryStorageAllocator<T>>(std::forward<decltype(args)>(args)...);
}

template<typename T>
    requires(std::is_unbounded_array_v<T>)
[[nodiscard]] auto make_temporary_for_overwrite(usize count) -> Temporary<T>
{
    return make_unique_for_overwrite<T, TemporaryStorageAllocator<std::remove_extent_t<T>>>(count);
}

template<typename... Args>
[[nodiscard]] auto format_to_temporary(fmt::format_string<Args...> fmt, Args&&... args) -> TemporaryString
{
    TemporaryString result;
    format_to(std::back_inserter(result), fmt, std::forward<decltype(args)>(args)...);
    return result;
}

template<typename T> auto TemporaryStorageAllocator<T>::allocate(std::size_t count) const noexcept -> T*
{
    return static_cast<T*>(TemporaryStorage::allocate(count * sizeof(T), alignof(T)));
}

template<typename T>
auto TemporaryStorageAllocator<T>::deallocate([[maybe_unused]] T* ptr,
                                              [[maybe_unused]] std::size_t count) const noexcept -> void
{}

} // namespace zth
