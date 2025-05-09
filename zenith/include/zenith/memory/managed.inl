#pragma once

#include <utility>

#include "zenith/core/assert.hpp"

namespace zth {

template<typename T, memory::StatelessAllocator A> UniquePtr<T, A>::UniquePtr(std::nullptr_t) noexcept {}

template<typename T, memory::StatelessAllocator A> auto UniquePtr<T, A>::null() noexcept -> UniquePtr
{
    return UniquePtr{};
}

template<typename T, memory::StatelessAllocator A> auto UniquePtr<T, A>::construct_new(auto&&... args) -> UniquePtr
{
    auto* ptr = memory::allocate_and_construct_object_using_allocator(A{}, std::forward<decltype(args)>(args)...);
    ZTH_ASSERT(ptr != nullptr);
    return UniquePtr{ ptr };
}

template<typename T, memory::StatelessAllocator A> auto UniquePtr<T, A>::allocate_for_overwrite() -> UniquePtr
{
    auto* ptr = memory::allocate_using_allocator(A{});
    ZTH_ASSERT(ptr != nullptr);
    return UniquePtr{ ptr };
}

template<typename T, memory::StatelessAllocator A> UniquePtr<T, A>::~UniquePtr()
{
    free();
}

template<typename T, memory::StatelessAllocator A>
UniquePtr<T, A>::UniquePtr(UniquePtr&& other) noexcept : _ptr{ std::exchange(other._ptr, nullptr) }
{}

template<typename T, memory::StatelessAllocator A>
template<typename U, memory::StatelessAllocator B>
    requires(std::convertible_to<U*, T*> && std::convertible_to<B, A>)
UniquePtr<T, A>::UniquePtr(UniquePtr<U, B>&& other) noexcept : _ptr{ std::exchange(other._ptr, nullptr) }
{}

template<typename T, memory::StatelessAllocator A>
auto UniquePtr<T, A>::operator=(UniquePtr&& other) noexcept -> UniquePtr&
{
    free();
    _ptr = std::exchange(other._ptr, nullptr);
    return *this;
}

template<typename T, memory::StatelessAllocator A>
template<typename U, memory::StatelessAllocator B>
    requires(std::convertible_to<U*, T*> && std::convertible_to<B, A>)
auto UniquePtr<T, A>::operator=(UniquePtr<U, B>&& other) noexcept -> UniquePtr&
{
    free();
    _ptr = std::exchange(other._ptr, nullptr);
    return *this;
}

template<typename T, memory::StatelessAllocator A> UniquePtr<T, A>::operator bool() const noexcept
{
    return _ptr != nullptr;
}

template<typename T, memory::StatelessAllocator A> auto UniquePtr<T, A>::operator*() const noexcept -> T&
{
    ZTH_ASSERT(_ptr != nullptr);
    return *_ptr;
}

template<typename T, memory::StatelessAllocator A> auto UniquePtr<T, A>::operator->() const noexcept -> T*
{
    ZTH_ASSERT(_ptr != nullptr);
    return _ptr;
}

template<typename T, memory::StatelessAllocator A>
auto UniquePtr<T, A>::operator==(std::nullptr_t) const noexcept -> bool
{
    return _ptr == nullptr;
}

template<typename T, memory::StatelessAllocator A> auto UniquePtr<T, A>::free() -> void
{
    if (!_ptr)
        return;

    memory::destroy_and_deallocate_object_using_allocator(A{}, _ptr);
    _ptr = nullptr;
}

template<typename T, memory::StatelessAllocator A> auto UniquePtr<T, A>::get() const noexcept -> T*
{
    return _ptr;
}

template<typename T, memory::StatelessAllocator A> UniquePtr<T, A>::UniquePtr(T* ptr) noexcept : _ptr{ ptr } {}

template<typename T, memory::StatelessAllocator A> UniquePtr<T[], A>::UniquePtr(std::nullptr_t) noexcept {}

template<typename T, memory::StatelessAllocator A> auto UniquePtr<T[], A>::null() noexcept -> UniquePtr
{
    return UniquePtr{};
}

template<typename T, memory::StatelessAllocator A> auto UniquePtr<T[], A>::construct_new(usize count) -> UniquePtr
{
    auto* ptr = memory::allocate_and_construct_objects_using_allocator(A{}, count);

#if defined(ZTH_ASSERTIONS)
    if (count != 0)
        ZTH_ASSERT(ptr != nullptr);
#endif

    return UniquePtr{ ptr, count };
}

template<typename T, memory::StatelessAllocator A>
auto UniquePtr<T[], A>::allocate_for_overwrite(usize count) -> UniquePtr
{
    auto* ptr = memory::allocate_using_allocator(A{}, count);

#if defined(ZTH_ASSERTIONS)
    if (count != 0)
        ZTH_ASSERT(ptr != nullptr);
#endif

    return UniquePtr{ ptr, count };
}

template<typename T, memory::StatelessAllocator A> UniquePtr<T[], A>::~UniquePtr()
{
    free();
}

template<typename T, memory::StatelessAllocator A>
UniquePtr<T[], A>::UniquePtr(UniquePtr&& other) noexcept
    : _ptr{ std::exchange(other._ptr, nullptr) }, _count{ std::exchange(other._count, 0) }
{}

template<typename T, memory::StatelessAllocator A>
template<typename U, memory::StatelessAllocator B>
    requires(!std::is_unbounded_array_v<U> && std::convertible_to<U*, T*> && std::convertible_to<B, A>)
UniquePtr<T[], A>::UniquePtr(UniquePtr<U, B>&& other) noexcept : _ptr{ std::exchange(other._ptr, nullptr) }, _count{ 1 }
{}

template<typename T, memory::StatelessAllocator A>
template<memory::StatelessAllocator B>
    requires(std::convertible_to<B, A>)
UniquePtr<T[], A>::UniquePtr(UniquePtr<T[], B>&& other) noexcept
    : _ptr{ std::exchange(other._ptr, nullptr) }, _count{ std::exchange(other._count, 0) }
{}

template<typename T, memory::StatelessAllocator A>
auto UniquePtr<T[], A>::operator=(UniquePtr&& other) noexcept -> UniquePtr&
{
    free();
    _ptr = std::exchange(other._ptr, nullptr);
    _count = std::exchange(other._count, 0);
    return *this;
}

template<typename T, memory::StatelessAllocator A>
template<typename U, memory::StatelessAllocator B>
    requires(!std::is_unbounded_array_v<U> && std::convertible_to<U*, T*> && std::convertible_to<B, A>)
auto UniquePtr<T[], A>::operator=(UniquePtr<U, B>&& other) noexcept -> UniquePtr&
{
    free();
    _ptr = std::exchange(other._ptr, nullptr);
    _count = 1;
    return *this;
}

template<typename T, memory::StatelessAllocator A>
template<memory::StatelessAllocator B>
    requires(std::convertible_to<B, A>)
auto UniquePtr<T[], A>::operator=(UniquePtr<T[], B>&& other) noexcept -> UniquePtr&
{
    free();
    _ptr = std::exchange(other._ptr, nullptr);
    _count = std::exchange(other._count, 0);
    return *this;
}

template<typename T, memory::StatelessAllocator A> UniquePtr<T[], A>::operator bool() const noexcept
{
    return _ptr != nullptr;
}

template<typename T, memory::StatelessAllocator A> auto UniquePtr<T[], A>::operator[](usize idx) const noexcept -> T&
{
    ZTH_ASSERT(idx < _count);
    return _ptr[idx];
}

template<typename T, memory::StatelessAllocator A>
auto UniquePtr<T[], A>::operator==(std::nullptr_t) const noexcept -> bool
{
    return _ptr == nullptr;
}

template<typename T, memory::StatelessAllocator A> auto UniquePtr<T[], A>::free() -> void
{
    if (!_ptr)
        return;

    memory::destroy_and_deallocate_objects_using_allocator(A{}, _ptr, _count);
    _ptr = nullptr;
    _count = 0;
}

template<typename T, memory::StatelessAllocator A> auto UniquePtr<T[], A>::get() const noexcept -> T*
{
    return _ptr;
}

template<typename T, memory::StatelessAllocator A> auto UniquePtr<T[], A>::count() const noexcept -> usize
{
    return _count;
}

template<typename T, memory::StatelessAllocator A>
UniquePtr<T[], A>::UniquePtr(T* ptr, usize count) noexcept : _ptr{ ptr }, _count{ count }
{}

template<typename T, memory::StatelessAllocator A>
    requires(!std::is_unbounded_array_v<T>)
auto make_unique(auto&&... args) -> UniquePtr<T, A>
{
    return UniquePtr<T, A>::construct_new(std::forward<decltype(args)>(args)...);
}

template<typename T, memory::StatelessAllocator A>
    requires(std::is_unbounded_array_v<T>)
auto make_unique(usize count) -> UniquePtr<T, A>
{
    return UniquePtr<T, A>::construct_new(count);
}

template<typename T, memory::StatelessAllocator A>
    requires(!std::is_unbounded_array_v<T>)
auto make_unique_for_overwrite() -> UniquePtr<T, A>
{
    return UniquePtr<T, A>::allocate_for_overwrite();
}

template<typename T, memory::StatelessAllocator A>
    requires(std::is_unbounded_array_v<T>)
auto make_unique_for_overwrite(usize count) -> UniquePtr<T, A>
{
    return UniquePtr<T, A>::allocate_for_overwrite(count);
}

} // namespace zth
