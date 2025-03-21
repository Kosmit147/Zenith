#pragma once

#include <concepts>
#include <memory>
#include <type_traits>

#include "zenith/core/assert.hpp"
#include "zenith/core/typedefs.hpp"
#include "zenith/util/defer.hpp"

namespace zth::memory {

// Custom allocation functions, which call allocation functions from C standard library under the hood, but don't
// exhibit any implementation-defined behavior.

[[nodiscard]] auto allocate(usize size_bytes) noexcept -> void*;
// Modifies ptr to point to the new location.
auto reallocate(auto*& ptr, usize new_size_bytes) noexcept -> void;
// Modifies ptr to point to the new location. buffer_size_bytes determines how many bytes will be copied to the new
// destination.
auto reallocate(auto*& ptr, usize new_size_bytes, usize buffer_size_bytes) noexcept -> void;
// This function sets ptr to nullptr.
auto deallocate(auto*& ptr) noexcept -> void;

// Destructs an object and deallocates the memory using our custom allocation functions. Handles pointers to arrays, but
// not arrays of dynamic size (T[]).
// @todo: Handle arrays of dynamic size (T[]). We should take into account whether the type is trivially destructible,
// because then we don't have to actually store the additional metadata nor call any destructors. We could either store
// the information about the number of objects to destruct the same way the standard operator delete[] does, or store
// the number of allocated objects as part of the state of the deleter.
template<std::destructible T>
    requires(!std::is_unbounded_array_v<T>)
struct CustomDeleter
{
public:
    CustomDeleter() noexcept = default;

    template<typename U>
    CustomDeleter(const CustomDeleter<U>&) noexcept
        requires(std::convertible_to<U*, T*>)
    {}

    // std::destructible ensures that T's destructor doesn't throw.
    auto operator()(T* ptr) const noexcept -> void;
};

template<typename T> struct CustomAllocator
{
    // A minimal allocator implementation.

    using value_type = T;

    CustomAllocator() noexcept = default;
    template<typename U> CustomAllocator(const CustomAllocator<U>&) noexcept {}
    template<typename U> auto operator==(const CustomAllocator<U>&) const noexcept -> bool { return true; }
    template<typename U> auto operator!=(const CustomAllocator<U>&) const noexcept -> bool { return false; }

    auto allocate(std::size_t count) const noexcept -> T*;
    auto deallocate(T* ptr, std::size_t count) const noexcept -> void;
};

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
#if !defined(ZTH_DIST_BUILD)
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

template<std::destructible T>
    requires(!std::is_unbounded_array_v<T>)
auto CustomDeleter<T>::operator()(T* ptr) const noexcept -> void
{
    std::destroy_at(ptr);
    deallocate(ptr);
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
