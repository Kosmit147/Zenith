#pragma once

#include <iterator>
#include <memory>
#include <utility>

#include "zenith/core/assert.hpp"

namespace zth {

template<std::movable T, usize Capacity>
constexpr InPlaceVector<T, Capacity>::InPlaceVector(size_type count)
    noexcept(std::is_nothrow_default_constructible_v<value_type>)
{
    ZTH_ASSERT(count <= capacity());
    _end = std::uninitialized_default_construct_n(begin(), count);
}

template<std::movable T, usize Capacity>
constexpr InPlaceVector<T, Capacity>::InPlaceVector(size_type count, const value_type& value)
    noexcept(std::is_nothrow_copy_constructible_v<value_type>)
{
    ZTH_ASSERT(count <= capacity());
    _end = std::uninitialized_fill_n(begin(), count, value);
}

template<std::movable T, usize Capacity>
constexpr InPlaceVector<T, Capacity>::InPlaceVector(std::initializer_list<value_type> values)
    noexcept(std::is_nothrow_copy_constructible_v<value_type>)
{
    ZTH_ASSERT(values.size() <= capacity());
    _end = std::uninitialized_copy(values.begin(), values.end(), begin());
}

template<std::movable T, usize Capacity>
constexpr InPlaceVector<T, Capacity>::InPlaceVector(const InPlaceVector& other)
    noexcept(std::is_nothrow_copy_constructible_v<value_type>)
{
    _end = std::uninitialized_copy(other.begin(), other.end(), begin());
}

template<std::movable T, usize Capacity>
constexpr auto InPlaceVector<T, Capacity>::operator=(const InPlaceVector& other)
    noexcept(std::is_nothrow_copy_constructible_v<value_type> && std::is_nothrow_destructible_v<value_type>)
        -> InPlaceVector&
{
    if (this == &other)
        return *this;

    clear();
    _end = std::uninitialized_copy(other.begin(), other.end(), begin());

    return *this;
}

template<std::movable T, usize Capacity>
constexpr InPlaceVector<T, Capacity>::InPlaceVector(InPlaceVector&& other)
    noexcept(std::is_nothrow_move_constructible_v<value_type>)
{
    _end = std::uninitialized_move(other.begin(), other.end(), begin());
}

template<std::movable T, usize Capacity>
constexpr auto InPlaceVector<T, Capacity>::operator=(InPlaceVector&& other)
    noexcept(std::is_nothrow_move_constructible_v<value_type> && std::is_nothrow_destructible_v<value_type>)
        -> InPlaceVector&
{
    clear();
    _end = std::uninitialized_move(other.begin(), other.end(), begin());

    return *this;
}

template<std::movable T, usize Capacity>
constexpr InPlaceVector<T, Capacity>::~InPlaceVector() noexcept(std::is_nothrow_destructible_v<value_type>)
{
    clear();
}

template<std::movable T, usize Capacity>
constexpr auto InPlaceVector<T, Capacity>::data(this auto&& self) noexcept -> decltype(auto)
{
    using return_type =
        std::conditional_t<std::is_const_v<std::remove_reference_t<decltype(self)>>, const_pointer, pointer>;

    return reinterpret_cast<return_type>(self._data.data());
}

template<std::movable T, usize Capacity>
constexpr auto InPlaceVector<T, Capacity>::cbegin(this auto&& self) noexcept -> decltype(auto)
{
    return self.data();
}

template<std::movable T, usize Capacity>
constexpr auto InPlaceVector<T, Capacity>::cend(this auto&& self) noexcept -> decltype(auto)
{
    ZTH_ASSERT(self._end != nullptr);

    using return_type =
        std::conditional_t<std::is_const_v<std::remove_reference_t<decltype(self)>>, const_pointer, pointer>;

    return reinterpret_cast<return_type>(self._end);
}

template<std::movable T, usize Capacity> constexpr auto InPlaceVector<T, Capacity>::size() const noexcept -> size_type
{
    return std::distance(begin(), end());
}

template<std::movable T, usize Capacity> constexpr auto InPlaceVector<T, Capacity>::capacity() noexcept -> size_type
{
    return Capacity;
}

template<std::movable T, usize Capacity>
template<typename... Args>
constexpr auto InPlaceVector<T, Capacity>::emplace_back(Args&&... args)
    noexcept(std::is_nothrow_constructible_v<value_type, Args...>) -> value_type&
{
    ZTH_ASSERT(size() < capacity());
    auto last = std::construct_at(end(), std::forward<decltype(args)>(args)...);
    ++_end;
    return *last;
}

template<std::movable T, usize Capacity>
constexpr auto InPlaceVector<T, Capacity>::push_back(const value_type& value)
    noexcept(std::is_nothrow_copy_constructible_v<value_type>) -> value_type&
{
    ZTH_ASSERT(size() < capacity());
    auto last = std::construct_at(end(), value);
    ++_end;
    return *last;
}

template<std::movable T, usize Capacity>
constexpr auto InPlaceVector<T, Capacity>::push_back(value_type&& value)
    noexcept(std::is_nothrow_move_constructible_v<value_type>) -> value_type&
{
    ZTH_ASSERT(size() < capacity());
    auto last = std::construct_at(end(), std::move(value));
    ++_end;
    return *last;
}

template<std::movable T, usize Capacity>
constexpr auto InPlaceVector<T, Capacity>::pop_back() noexcept(std::is_nothrow_destructible_v<value_type>) -> void
{
    ZTH_ASSERT(!empty());
    std::destroy_at(std::prev(end()));
    --_end;
}

template<std::movable T, usize Capacity>
constexpr auto InPlaceVector<T, Capacity>::clear() noexcept(std::is_nothrow_destructible_v<value_type>) -> void
{
    std::ranges::destroy(*this);
    _end = begin();
}

} // namespace zth
