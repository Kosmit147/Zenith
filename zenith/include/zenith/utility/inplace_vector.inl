#pragma once

#include <memory>
#include <ranges>
#include <utility>

#include "zenith/core/assert.hpp"
#include "zenith/core/exception.hpp"

namespace zth {

template<std::movable T, usize Size>
constexpr InPlaceVector<T, Size>::InPlaceVector(usize count) noexcept(std::is_nothrow_default_constructible_v<T>)
{
    ZTH_ASSERT(count <= Size);
    _size = count;
    std::ranges::uninitialized_default_construct(*this);
}

template<std::movable T, usize Size>
constexpr InPlaceVector<T, Size>::InPlaceVector(usize count, const T& value)
    noexcept(std::is_nothrow_copy_constructible_v<T>)
{
    ZTH_ASSERT(count <= Size);
    _size = count;
    std::ranges::uninitialized_fill(*this, value);
}

template<std::movable T, usize Size>
constexpr InPlaceVector<T, Size>::InPlaceVector(std::initializer_list<T> values)
    noexcept(std::is_nothrow_copy_constructible_v<T>)
{
    ZTH_ASSERT(values.size() <= Size);
    _size = values.size();
    std::ranges::uninitialized_copy(values, *this);
}

template<std::movable T, usize Size>
constexpr InPlaceVector<T, Size>::InPlaceVector(const InPlaceVector& other)
    noexcept(std::is_nothrow_copy_constructible_v<T>)
{
    _size = other.size();
    std::ranges::uninitialized_copy(other, *this);
}

template<std::movable T, usize Size>
constexpr auto InPlaceVector<T, Size>::operator=(const InPlaceVector& other)
    noexcept(std::is_nothrow_copy_constructible_v<T> && std::is_nothrow_destructible_v<T>) -> InPlaceVector&
{
    if (this == &other)
        return *this;

    clear();

    _size = other.size();
    std::ranges::uninitialized_copy(other, *this);

    return *this;
}

template<std::movable T, usize Size>
constexpr InPlaceVector<T, Size>::InPlaceVector(InPlaceVector&& other) noexcept(std::is_nothrow_move_constructible_v<T>)
{
    _size = other.size();
    std::ranges::uninitialized_move(other, *this);

    other.clear();
}

template<std::movable T, usize Size>
constexpr auto InPlaceVector<T, Size>::operator=(InPlaceVector&& other)
    noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_destructible_v<T>) -> InPlaceVector&
{
    clear();

    _size = other.size();
    std::ranges::uninitialized_move(other, *this);

    other.clear();

    return *this;
}

template<std::movable T, usize Size>
constexpr InPlaceVector<T, Size>::~InPlaceVector() noexcept(std::is_nothrow_destructible_v<T>)
{
    clear();
}

template<std::movable T, usize Size> constexpr auto InPlaceVector<T, Size>::at(usize index) -> T&
{
    if (index >= _size)
        throw Exception{ "Index out of range." };

    return data()[index];
}

template<std::movable T, usize Size> constexpr auto InPlaceVector<T, Size>::at(usize index) const -> const T&
{
    if (index >= _size)
        throw Exception{ "Index out of range." };

    return data()[index];
}

template<std::movable T, usize Size> constexpr auto InPlaceVector<T, Size>::operator[](usize index) noexcept -> T&
{
    ZTH_ASSERT(index < _size);
    return data()[index];
}

template<std::movable T, usize Size>
constexpr auto InPlaceVector<T, Size>::operator[](usize index) const noexcept -> const T&
{
    ZTH_ASSERT(index < _size);
    return data()[index];
}

template<std::movable T, usize Size> constexpr auto InPlaceVector<T, Size>::front() noexcept -> T&
{
    ZTH_ASSERT(_size != 0);
    return *data();
}

template<std::movable T, usize Size> constexpr auto InPlaceVector<T, Size>::front() const noexcept -> const T&
{
    ZTH_ASSERT(_size != 0);
    return *data();
}

template<std::movable T, usize Size> constexpr auto InPlaceVector<T, Size>::back() noexcept -> T&
{
    ZTH_ASSERT(_size != 0);
    return data()[_size - 1];
}

template<std::movable T, usize Size> constexpr auto InPlaceVector<T, Size>::back() const noexcept -> const T&
{
    ZTH_ASSERT(_size != 0);
    return data()[_size - 1];
}

template<std::movable T, usize Size> constexpr auto InPlaceVector<T, Size>::data() noexcept -> T*
{
    return reinterpret_cast<T*>(_data.data());
}

template<std::movable T, usize Size> constexpr auto InPlaceVector<T, Size>::data() const noexcept -> const T*
{
    return reinterpret_cast<const T*>(_data.data());
}

template<std::movable T, usize Size> constexpr auto InPlaceVector<T, Size>::begin() noexcept -> T*
{
    return cbegin();
}

template<std::movable T, usize Size> constexpr auto InPlaceVector<T, Size>::begin() const noexcept -> const T*
{
    return cbegin();
}

template<std::movable T, usize Size> constexpr auto InPlaceVector<T, Size>::cbegin() noexcept -> T*
{
    return data();
}

template<std::movable T, usize Size> constexpr auto InPlaceVector<T, Size>::cbegin() const noexcept -> const T*
{
    return data();
}

template<std::movable T, usize Size> constexpr auto InPlaceVector<T, Size>::end() noexcept -> T*
{
    return cend();
}

template<std::movable T, usize Size> constexpr auto InPlaceVector<T, Size>::end() const noexcept -> const T*
{
    return cend();
}

template<std::movable T, usize Size> constexpr auto InPlaceVector<T, Size>::cend() noexcept -> T*
{
    return cbegin() + _size;
}

template<std::movable T, usize Size> constexpr auto InPlaceVector<T, Size>::cend() const noexcept -> const T*
{
    return cbegin() + _size;
}

template<std::movable T, usize Size> constexpr auto InPlaceVector<T, Size>::empty() const noexcept -> bool
{
    return _size == 0;
}

template<std::movable T, usize Size> constexpr auto InPlaceVector<T, Size>::size() const noexcept -> usize
{
    return _size;
}

template<std::movable T, usize Size> constexpr auto InPlaceVector<T, Size>::max_size() noexcept -> usize
{
    return Size;
}

template<std::movable T, usize Size> constexpr auto InPlaceVector<T, Size>::capacity() noexcept -> usize
{
    return Size;
}

template<std::movable T, usize Size>
template<typename... Args>
constexpr auto InPlaceVector<T, Size>::emplace_back(Args&&... args)
    noexcept(std::is_nothrow_constructible_v<T, Args...>) -> T&
{
    ZTH_ASSERT(_size < capacity());
    std::construct_at(cbegin() + _size, std::forward<Args>(args)...);
    _size++;
    return back();
}

template<std::movable T, usize Size>
constexpr auto InPlaceVector<T, Size>::push_back(const T& value) noexcept(std::is_nothrow_copy_constructible_v<T>) -> T&
{
    ZTH_ASSERT(_size < capacity());
    std::construct_at(cbegin() + _size, value);
    _size++;
    return back();
}

template<std::movable T, usize Size>
constexpr auto InPlaceVector<T, Size>::push_back(T&& value) noexcept(std::is_nothrow_move_constructible_v<T>) -> T&
{
    ZTH_ASSERT(_size < capacity());
    std::construct_at(cbegin() + _size, std::move(value));
    _size++;
    return back();
}

template<std::movable T, usize Size>
constexpr auto InPlaceVector<T, Size>::pop_back() noexcept(std::is_nothrow_destructible_v<T>) -> void
{
    ZTH_ASSERT(_size != 0);
    _size--;
    std::destroy_at(cend());
}

template<std::movable T, usize Size>
constexpr auto InPlaceVector<T, Size>::clear() noexcept(std::is_nothrow_destructible_v<T>) -> void
{
    std::destroy(begin(), end());
    _size = 0;
}

} // namespace zth
