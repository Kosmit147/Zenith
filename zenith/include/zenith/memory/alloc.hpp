#pragma once

#include <concepts>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <type_traits>
#include <utility>

#include "zenith/core/assert.hpp"
#include "zenith/core/typedefs.hpp"
#include "zenith/util/defer.hpp"

namespace zth::memory {

template<typename A>
concept Allocator = requires(A allocator, usize n) {
    { *allocator.allocate(n) } -> std::same_as<typename std::remove_cvref_t<A>::value_type&>;
    { allocator.deallocate(allocator.allocate(n), n) };
} && std::copy_constructible<A> && std::equality_comparable<A>;

template<typename A>
concept StatelessAllocator = Allocator<A> && std::is_empty_v<A>;

template<typename T, typename Alloc>
concept AllocatableWith = Allocator<Alloc> && std::same_as<T, typename std::remove_cvref_t<Alloc>::value_type>;

template<Allocator A> [[nodiscard]] auto allocate_using_allocator(A&& allocator, usize count = 1) -> auto*;

template<Allocator A, typename T>
    requires AllocatableWith<T, A>
auto deallocate_using_allocator(A&& allocator, T* memory, usize count = 1) -> void;

template<Allocator A, typename T>
    requires AllocatableWith<T, A>
auto construct_object_using_allocator(A&& allocator, T* location, auto&&... args) -> void;

template<Allocator A, typename T>
    requires AllocatableWith<T, A>
auto construct_objects_using_allocator(A&& allocator, T* location, usize count) -> void;

template<Allocator A, typename T>
    requires AllocatableWith<T, A>
auto destroy_object_using_allocator(A&& allocator, T* location) -> void;

template<Allocator A, typename T>
    requires AllocatableWith<T, A>
auto destroy_objects_using_allocator(A&& allocator, T* location, usize count) -> void;

// Assumes that the allocator doesn't fail.
template<Allocator A>
[[nodiscard]] auto allocate_and_construct_object_using_allocator(A&& allocator, auto&&... args) -> auto*;

// Assumes that the allocator doesn't fail.
template<Allocator A>
[[nodiscard]] auto allocate_and_construct_objects_using_allocator(A&& allocator, usize count) -> auto*;

template<Allocator A, typename T>
    requires AllocatableWith<T, A>
auto destroy_and_deallocate_object_using_allocator(A&& allocator, T* location) -> void;

template<Allocator A, typename T>
    requires AllocatableWith<T, A>
auto destroy_and_deallocate_objects_using_allocator(A&& allocator, T* location, usize count) -> void;

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

template<Allocator A> auto allocate_using_allocator(A&& allocator, usize count) -> auto*
{
    return std::allocator_traits<std::remove_cvref_t<decltype(allocator)>>::allocate(allocator, count);
}

template<Allocator A, typename T>
    requires AllocatableWith<T, A>
auto deallocate_using_allocator(A&& allocator, T* memory, usize count) -> void
{
    std::allocator_traits<std::remove_cvref_t<decltype(allocator)>>::deallocate(allocator, memory, count);
}

template<Allocator A, typename T>
    requires AllocatableWith<T, A>
auto construct_object_using_allocator(A&& allocator, T* location, auto&&... args) -> void
{
    ZTH_ASSERT(location != nullptr);
    std::allocator_traits<std::remove_cvref_t<decltype(allocator)>>::construct(allocator, location,
                                                                               std::forward<decltype(args)>(args)...);
}

template<Allocator A, typename T>
    requires AllocatableWith<T, A>
auto construct_objects_using_allocator(A&& allocator, T* location, usize count) -> void
{
#if defined(ZTH_ASSERTIONS)
    if (count != 0)
        ZTH_ASSERT(location != nullptr);
#endif

    auto end = location + count;
    for (; location != end; ++location)
        construct_object_using_allocator(allocator, location);
}

template<Allocator A, typename T>
    requires AllocatableWith<T, A>
auto destroy_object_using_allocator(A&& allocator, T* location) -> void
{
    ZTH_ASSERT(location != nullptr);
    std::allocator_traits<std::remove_cvref_t<decltype(allocator)>>::destroy(allocator, location);
}

template<Allocator A, typename T>
    requires AllocatableWith<T, A>
auto destroy_objects_using_allocator(A&& allocator, T* location, usize count) -> void
{
#if defined(ZTH_ASSERTIONS)
    if (count != 0)
        ZTH_ASSERT(location != nullptr);
#endif

    auto end = location + count;
    for (; location != end; ++location)
        destroy_object_using_allocator(allocator, location);
}

template<Allocator A> auto allocate_and_construct_object_using_allocator(A&& allocator, auto&&... args) -> auto*
{
    auto* location = allocate_using_allocator(allocator);
    ZTH_ASSERT(location != nullptr);
    construct_object_using_allocator(allocator, location, std::forward<decltype(args)>(args)...);
    return location;
}

template<Allocator A> auto allocate_and_construct_objects_using_allocator(A&& allocator, usize count) -> auto*
{
    auto* location = allocate_using_allocator(allocator, count);

#if defined(ZTH_ASSERTIONS)
    if (count != 0)
        ZTH_ASSERT(location != nullptr);
#endif

    construct_objects_using_allocator(allocator, location, count);
    return location;
}

template<Allocator A, typename T>
    requires AllocatableWith<T, A>
auto destroy_and_deallocate_object_using_allocator(A&& allocator, T* location) -> void
{
    ZTH_ASSERT(location != nullptr);
    destroy_object_using_allocator(allocator, location);
    deallocate_using_allocator(allocator, location);
}

template<Allocator A, typename T>
    requires AllocatableWith<T, A>
auto destroy_and_deallocate_objects_using_allocator(A&& allocator, T* location, usize count) -> void
{
#if defined(ZTH_ASSERTIONS)
    if (count != 0)
        ZTH_ASSERT(location != nullptr);
#endif

    destroy_objects_using_allocator(allocator, location, count);
    deallocate_using_allocator(allocator, location, count);
}

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
