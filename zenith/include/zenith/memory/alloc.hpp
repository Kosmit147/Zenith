#pragma once

#include <concepts>
#include <cstdlib>
#include <cstring>
#include <type_traits>

#include "zenith/core/assert.hpp"
#include "zenith/core/typedefs.hpp"
#include "zenith/util/defer.hpp"

namespace zth::memory {

template<typename A>
concept Allocator = requires(A allocator, usize n) {
    { *allocator.allocate(n) } -> std::same_as<typename A::value_type&>;
    { allocator.deallocate(allocator.allocate(n), n) };
} && std::copy_constructible<A> && std::equality_comparable<A>;

template<typename A>
concept StatelessAllocator = Allocator<A> && std::is_empty_v<A>;

// Custom allocation functions, which call allocation functions from C standard library under the hood, but don't
// exhibit any implementation-defined behavior.

[[nodiscard]] auto allocate(usize size_bytes) noexcept -> void*;
[[nodiscard]] auto allocate_zeroed(usize size_bytes) noexcept -> void*;
// Modifies ptr to point to the new location.
auto reallocate(auto*& ptr, usize new_size_bytes) noexcept -> void;
// Modifies ptr to point to the new location. buffer_size_bytes determines how many bytes will be copied to the new
// destination.
auto reallocate(auto*& ptr, usize new_size_bytes, usize buffer_size_bytes) noexcept -> void;
// This function sets ptr to nullptr.
auto deallocate(auto*& ptr) noexcept -> void;

template<typename T> struct CustomAllocator
{
    // A minimal allocator implementation.
    static_assert(!std::is_const_v<T>);
    static_assert(!std::is_function_v<T>);
    static_assert(!std::is_reference_v<T>);

    using value_type = T;

    CustomAllocator() noexcept = default;
    template<typename U> CustomAllocator(const CustomAllocator<U>&) noexcept {}
    template<typename U> auto operator==(const CustomAllocator<U>&) const noexcept -> bool { return true; }
    template<typename U> auto operator!=(const CustomAllocator<U>&) const noexcept -> bool { return false; }

    auto allocate(std::size_t count) const noexcept -> T*;
    auto deallocate(T* ptr, std::size_t count) const noexcept -> void;
};

static_assert(StatelessAllocator<CustomAllocator<int>>);

auto reallocate(auto*& ptr, usize new_size_bytes) noexcept -> void
{
    if (new_size_bytes == 0) [[unlikely]]
    {
        deallocate(ptr);
        return;
    }

    ptr = std::realloc(ptr, new_size_bytes);
    ZTH_ASSERT(ptr != nullptr);
}

auto reallocate(auto*& ptr, usize new_size_bytes, usize buffer_size_bytes) noexcept -> void
{
#if defined(ZTH_ASSERTIONS)
    if (ptr == nullptr)
        ZTH_ASSERT(buffer_size_bytes == 0);
#endif

    auto old_ptr = ptr;
    Defer deallocate_old_ptr{ [&] { deallocate(old_ptr); } };

    // @speed: Maybe using std::realloc would be faster.
    // The rationale for what we're doing right now is that buffer_size_bytes could potentially be a lot smaller than
    // the old size of the allocated block, and therefore we avoid unnecessarily copying memory.
    using PointerType = std::remove_reference_t<decltype(ptr)>;
    ptr = static_cast<PointerType>(allocate(new_size_bytes));
    std::memcpy(ptr, old_ptr, std::min(new_size_bytes, buffer_size_bytes));
}

auto deallocate(auto*& ptr) noexcept -> void
{
    std::free(ptr);
    ptr = nullptr;
}

template<typename T> auto CustomAllocator<T>::allocate(std::size_t count) const noexcept -> T*
{
    return static_cast<T*>(memory::allocate(count * sizeof(T)));
}

template<typename T>
auto CustomAllocator<T>::deallocate(T* ptr, [[maybe_unused]] std::size_t count) const noexcept -> void
{
    memory::deallocate(ptr);
}

} // namespace zth::memory
