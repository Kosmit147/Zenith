#pragma once

#include <memory>
#include <ranges>
#include <utility>

#include "zenith/core/assert.hpp"

namespace zth {

template<std::movable T, usize Capacity>
constexpr InPlaceVector<T, Capacity>::InPlaceVector(usize count) noexcept(std::is_nothrow_default_constructible_v<T>)
{
    ZTH_ASSERT(count <= capacity());
    _size = count;
    std::ranges::uninitialized_default_construct(*this);
}

template<std::movable T, usize Capacity>
constexpr InPlaceVector<T, Capacity>::InPlaceVector(usize count, const T& value)
    noexcept(std::is_nothrow_copy_constructible_v<T>)
{
    ZTH_ASSERT(count <= capacity());
    _size = count;
    std::ranges::uninitialized_fill(*this, value);
}

template<std::movable T, usize Capacity>
constexpr InPlaceVector<T, Capacity>::InPlaceVector(std::initializer_list<T> values)
    noexcept(std::is_nothrow_copy_constructible_v<T>)
{
    ZTH_ASSERT(values.size() <= capacity());
    _size = values.size();
    std::ranges::uninitialized_copy(values, *this);
}

template<std::movable T, usize Capacity>
constexpr InPlaceVector<T, Capacity>::InPlaceVector(const InPlaceVector& other)
    noexcept(std::is_nothrow_copy_constructible_v<T>)
{
    _size = other.size();
    std::ranges::uninitialized_copy(other, *this);
}

template<std::movable T, usize Capacity>
constexpr auto InPlaceVector<T, Capacity>::operator=(const InPlaceVector& other)
    noexcept(std::is_nothrow_copy_constructible_v<T> && std::is_nothrow_destructible_v<T>) -> InPlaceVector&
{
    if (this == &other)
        return *this;

    clear();

    _size = other.size();
    std::ranges::uninitialized_copy(other, *this);

    return *this;
}

template<std::movable T, usize Capacity>
constexpr InPlaceVector<T, Capacity>::InPlaceVector(InPlaceVector&& other)
    noexcept(std::is_nothrow_move_constructible_v<T>)
{
    _size = other.size();
    std::ranges::uninitialized_move(other, *this);

    other.clear();
}

template<std::movable T, usize Capacity>
constexpr auto InPlaceVector<T, Capacity>::operator=(InPlaceVector&& other)
    noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_destructible_v<T>) -> InPlaceVector&
{
    clear();

    _size = other.size();
    std::ranges::uninitialized_move(other, *this);

    other.clear();

    return *this;
}

template<std::movable T, usize Capacity>
constexpr InPlaceVector<T, Capacity>::~InPlaceVector() noexcept(std::is_nothrow_destructible_v<T>)
{
    clear();
}

template<std::movable T, usize Capacity>
constexpr auto InPlaceVector<T, Capacity>::at(usize index) noexcept -> std::optional<TRef>
{
    if (index >= _size)
        return {};

    return data()[index];
}

template<std::movable T, usize Capacity>
constexpr auto InPlaceVector<T, Capacity>::at(usize index) const noexcept -> std::optional<ConstTRef>
{
    if (index >= _size)
        return {};

    return data()[index];
}

template<std::movable T, usize Capacity>
constexpr auto InPlaceVector<T, Capacity>::operator[](usize index) noexcept -> T&
{
    ZTH_ASSERT(index < _size);
    return data()[index];
}

template<std::movable T, usize Capacity>
constexpr auto InPlaceVector<T, Capacity>::operator[](usize index) const noexcept -> const T&
{
    ZTH_ASSERT(index < _size);
    return data()[index];
}

template<std::movable T, usize Capacity> constexpr auto InPlaceVector<T, Capacity>::front() noexcept -> T&
{
    ZTH_ASSERT(_size != 0);
    return *data();
}

template<std::movable T, usize Capacity> constexpr auto InPlaceVector<T, Capacity>::front() const noexcept -> const T&
{
    ZTH_ASSERT(_size != 0);
    return *data();
}

template<std::movable T, usize Capacity> constexpr auto InPlaceVector<T, Capacity>::back() noexcept -> T&
{
    ZTH_ASSERT(_size != 0);
    return data()[_size - 1];
}

template<std::movable T, usize Capacity> constexpr auto InPlaceVector<T, Capacity>::back() const noexcept -> const T&
{
    ZTH_ASSERT(_size != 0);
    return data()[_size - 1];
}

template<std::movable T, usize Capacity> constexpr auto InPlaceVector<T, Capacity>::data() noexcept -> T*
{
    return reinterpret_cast<T*>(_data.data());
}

template<std::movable T, usize Capacity> constexpr auto InPlaceVector<T, Capacity>::data() const noexcept -> const T*
{
    return reinterpret_cast<const T*>(_data.data());
}

template<std::movable T, usize Capacity> constexpr auto InPlaceVector<T, Capacity>::begin() noexcept -> T*
{
    return cbegin();
}

template<std::movable T, usize Capacity> constexpr auto InPlaceVector<T, Capacity>::begin() const noexcept -> const T*
{
    return cbegin();
}

template<std::movable T, usize Capacity> constexpr auto InPlaceVector<T, Capacity>::cbegin() noexcept -> T*
{
    return data();
}

template<std::movable T, usize Capacity> constexpr auto InPlaceVector<T, Capacity>::cbegin() const noexcept -> const T*
{
    return data();
}

template<std::movable T, usize Capacity> constexpr auto InPlaceVector<T, Capacity>::end() noexcept -> T*
{
    return cend();
}

template<std::movable T, usize Capacity> constexpr auto InPlaceVector<T, Capacity>::end() const noexcept -> const T*
{
    return cend();
}

template<std::movable T, usize Capacity> constexpr auto InPlaceVector<T, Capacity>::cend() noexcept -> T*
{
    return cbegin() + _size;
}

template<std::movable T, usize Capacity> constexpr auto InPlaceVector<T, Capacity>::cend() const noexcept -> const T*
{
    return cbegin() + _size;
}

template<std::movable T, usize Capacity> constexpr auto InPlaceVector<T, Capacity>::empty() const noexcept -> bool
{
    return _size == 0;
}

template<std::movable T, usize Capacity> constexpr auto InPlaceVector<T, Capacity>::size() const noexcept -> usize
{
    return _size;
}

template<std::movable T, usize Capacity> constexpr auto InPlaceVector<T, Capacity>::max_size() noexcept -> usize
{
    return Capacity;
}

template<std::movable T, usize Capacity> constexpr auto InPlaceVector<T, Capacity>::capacity() noexcept -> usize
{
    return Capacity;
}

template<std::movable T, usize Capacity>
template<typename... Args>
constexpr auto InPlaceVector<T, Capacity>::emplace_back(Args&&... args)
    noexcept(std::is_nothrow_constructible_v<T, Args...>) -> T&
{
    ZTH_ASSERT(_size < capacity());
    std::construct_at(cbegin() + _size, std::forward<decltype(args)>(args)...);
    _size++;
    return back();
}

template<std::movable T, usize Capacity>
constexpr auto InPlaceVector<T, Capacity>::push_back(const T& value) noexcept(std::is_nothrow_copy_constructible_v<T>)
    -> T&
{
    ZTH_ASSERT(_size < capacity());
    std::construct_at(cbegin() + _size, value);
    _size++;
    return back();
}

template<std::movable T, usize Capacity>
constexpr auto InPlaceVector<T, Capacity>::push_back(T&& value) noexcept(std::is_nothrow_move_constructible_v<T>) -> T&
{
    ZTH_ASSERT(_size < capacity());
    std::construct_at(cbegin() + _size, std::move(value));
    _size++;
    return back();
}

template<std::movable T, usize Capacity>
constexpr auto InPlaceVector<T, Capacity>::pop_back() noexcept(std::is_nothrow_destructible_v<T>) -> void
{
    ZTH_ASSERT(_size != 0);
    _size--;
    std::destroy_at(cend());
}

template<std::movable T, usize Capacity>
constexpr auto InPlaceVector<T, Capacity>::clear() noexcept(std::is_nothrow_destructible_v<T>) -> void
{
    std::ranges::destroy(*this);
    _size = 0;
}

} // namespace zth
