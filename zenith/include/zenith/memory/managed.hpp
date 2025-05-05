#pragma once

#include <algorithm>
#include <compare>
#include <concepts>
#include <cstddef>
#include <memory>
#include <type_traits>

#include "zenith/core/typedefs.hpp"
#include "zenith/memory/alloc.hpp"
#include "zenith/util/macros.hpp"

namespace zth {

// An allocator-aware unique pointer. This unique pointer is implemented differently to the standard library's one as it
// doesn't just manage the object, it handles allocation and deallocation as well using the specified allocator. Assumes
// that the specified allocator doesn't fail.
template<typename T, memory::StatelessAllocator A = std::allocator<std::remove_extent_t<T>>> class UniquePtr
{
public:
    static_assert(std::same_as<T, typename A::value_type>,
                  "allocator's value type must be the same as unique pointer's value type");

    explicit UniquePtr() noexcept = default;
    UniquePtr(std::nullptr_t) noexcept;

    [[nodiscard]] static auto null() noexcept -> UniquePtr;
    [[nodiscard]] static auto construct_new(auto&&... args) -> UniquePtr;
    [[nodiscard]] static auto allocate_for_overwrite() -> UniquePtr;

    ~UniquePtr();

    ZTH_NO_COPY(UniquePtr)

    UniquePtr(UniquePtr&& other) noexcept;

    template<typename U, memory::StatelessAllocator B>
        requires(std::convertible_to<U*, T*> && std::convertible_to<B, A>)
    UniquePtr(UniquePtr<U, B>&& other) noexcept;

    auto operator=(UniquePtr&& other) noexcept -> UniquePtr&;

    template<typename U, memory::StatelessAllocator B>
        requires(std::convertible_to<U*, T*> && std::convertible_to<B, A>)
    auto operator=(UniquePtr<U, B>&& other) noexcept -> UniquePtr&;

    explicit operator bool() const noexcept;

    [[nodiscard]] auto operator*() const noexcept -> T&;
    [[nodiscard]] auto operator->() const noexcept -> T*;

    [[nodiscard]] auto operator<=>(const UniquePtr&) const -> std::strong_ordering = default;
    [[nodiscard]] auto operator==(std::nullptr_t) const noexcept -> bool;

    auto free() -> void;
    [[nodiscard]] auto get() const noexcept -> T*;

    friend auto swap(UniquePtr& first, UniquePtr& second) noexcept -> void { std::swap(first._ptr, second._ptr); }

    template<typename U, memory::StatelessAllocator B> friend class UniquePtr;

private:
    T* _ptr = nullptr;

private:
    explicit UniquePtr(T* ptr) noexcept;
};

// Partial specialization for unbounded arrays (T[]). Stores the number of allocated objects.
template<typename T, memory::StatelessAllocator A> class UniquePtr<T[], A>
{
public:
    static_assert(std::same_as<T, typename A::value_type>,
                  "allocator's value type must be the same as unique pointer's value type");

    explicit UniquePtr() noexcept = default;
    UniquePtr(std::nullptr_t) noexcept;

    [[nodiscard]] static auto null() noexcept -> UniquePtr;
    [[nodiscard]] static auto construct_new(usize count) -> UniquePtr;
    [[nodiscard]] static auto allocate_for_overwrite(usize count) -> UniquePtr;

    ~UniquePtr();

    ZTH_NO_COPY(UniquePtr)

    UniquePtr(UniquePtr&& other) noexcept;

    template<typename U, memory::StatelessAllocator B>
        requires(!std::is_unbounded_array_v<U> && std::convertible_to<U*, T*> && std::convertible_to<B, A>)
    UniquePtr(UniquePtr<U, B>&& other) noexcept;

    template<memory::StatelessAllocator B>
        requires(std::convertible_to<B, A>)
    UniquePtr(UniquePtr<T[], B>&& other) noexcept;

    auto operator=(UniquePtr&& other) noexcept -> UniquePtr&;

    template<typename U, memory::StatelessAllocator B>
        requires(!std::is_unbounded_array_v<U> && std::convertible_to<U*, T*> && std::convertible_to<B, A>)
    auto operator=(UniquePtr<U, B>&& other) noexcept -> UniquePtr&;

    template<memory::StatelessAllocator B>
        requires(std::convertible_to<B, A>)
    auto operator=(UniquePtr<T[], B>&& other) noexcept -> UniquePtr&;

    explicit operator bool() const noexcept;

    [[nodiscard]] auto operator[](usize idx) const noexcept -> T&;

    [[nodiscard]] auto operator<=>(const UniquePtr&) const -> std::strong_ordering = default;
    [[nodiscard]] auto operator==(std::nullptr_t) const noexcept -> bool;

    auto free() -> void;

    [[nodiscard]] auto get() const noexcept -> T*;
    [[nodiscard]] auto count() const noexcept -> usize;

    friend auto swap(UniquePtr& first, UniquePtr& second) noexcept -> void
    {
        std::swap(first._ptr, second._ptr);
        std::swap(first._count, second._count);
    }

    template<typename U, memory::StatelessAllocator B> friend class UniquePtr;

private:
    T* _ptr = nullptr;
    usize _count = 0;

private:
    explicit UniquePtr(T* ptr, usize count) noexcept;
};

template<typename T, memory::StatelessAllocator A = std::allocator<T>>
    requires(!std::is_unbounded_array_v<T>)
[[nodiscard]] auto make_unique(auto&&... args) -> UniquePtr<T, A>;

template<typename T, memory::StatelessAllocator A = std::allocator<std::remove_extent_t<T>>>
    requires(std::is_unbounded_array_v<T>)
[[nodiscard]] auto make_unique(usize count) -> UniquePtr<T, A>;

template<typename T, memory::StatelessAllocator A = std::allocator<T>>
    requires(!std::is_unbounded_array_v<T>)
[[nodiscard]] auto make_unique_for_overwrite() -> UniquePtr<T, A>;

template<typename T, memory::StatelessAllocator A = std::allocator<std::remove_extent_t<T>>>
    requires(std::is_unbounded_array_v<T>)
[[nodiscard]] auto make_unique_for_overwrite(usize count) -> UniquePtr<T, A>;

} // namespace zth

#include "managed.inl"
