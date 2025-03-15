#pragma once

#include <array>
#include <concepts>
#include <initializer_list>
#include <memory>
#include <type_traits>
#include <vector>

#include "zenith/core/typedefs.hpp"
#include "zenith/stl/range.hpp"

namespace zth {

// @todo: Implement iterators.
// @todo: Implement reverse iterators (rbegin, rend).

// A resizable array.
// @todo: Implement Vector.
template<std::movable T, typename Allocator = std::allocator<T>> using Vector = std::vector<T, Allocator>;

// A vector which stores its data on the stack, if the number of stored objects is not greater than Capacity, or on the
// heap otherwise. Implements contiguous range interface.
// Not currently implemented.
// @todo: Implement SmallVector.
template<std::movable T, usize Capacity, typename Allocator = std::allocator<T>>
using SmallVector = Vector<T, Allocator>;

// A vector which stores its data on the stack. The amount of stored objects cannot go above the specified Capacity.
// Implements contiguous range interface.
// @refactor: Replace with std::inplace_vector once we're on C++26.
template<std::movable T, usize Capacity> class InPlaceVector : public ContiguousRangeInterface
{
public:
    using value_type = T;
    using size_type = usize;
    using difference_type = isize;

    using pointer = value_type*;
    using const_pointer = const value_type*;

    using iterator = pointer;
    using const_iterator = const_pointer;

    constexpr InPlaceVector() noexcept;
    constexpr explicit InPlaceVector(size_type count) noexcept(std::is_nothrow_default_constructible_v<value_type>);
    constexpr explicit InPlaceVector(size_type count, const value_type& value)
        noexcept(std::is_nothrow_copy_constructible_v<value_type>);
    constexpr InPlaceVector(std::initializer_list<value_type> values)
        noexcept(std::is_nothrow_copy_constructible_v<value_type>);

    constexpr InPlaceVector(const InPlaceVector& other) noexcept(std::is_nothrow_copy_constructible_v<value_type>);
    constexpr auto operator=(const InPlaceVector& other)
        noexcept(std::is_nothrow_copy_constructible_v<value_type> && std::is_nothrow_destructible_v<value_type>)
            -> InPlaceVector&;

    constexpr InPlaceVector(InPlaceVector&& other) noexcept(std::is_nothrow_move_constructible_v<value_type>);
    constexpr auto operator=(InPlaceVector&& other)
        noexcept(std::is_nothrow_move_constructible_v<value_type> && std::is_nothrow_destructible_v<value_type>)
            -> InPlaceVector&;

    constexpr ~InPlaceVector() noexcept(std::is_nothrow_destructible_v<value_type>);

    // --- ContiguousRange implementation
    [[nodiscard]] constexpr auto data(this auto&& self) noexcept -> decltype(auto);
    [[nodiscard]] constexpr auto cbegin(this auto&& self) noexcept -> decltype(auto);
    [[nodiscard]] constexpr auto cend(this auto&& self) noexcept -> decltype(auto);
    [[nodiscard]] constexpr auto size() const noexcept -> size_type;
    [[nodiscard]] static constexpr auto capacity() noexcept -> size_type;

    // --- InPlaceVector implementation
    template<typename... Args>
    constexpr auto emplace_back(Args&&... args) noexcept(std::is_nothrow_constructible_v<value_type, Args...>)
        -> value_type&;
    constexpr auto push_back(const value_type& value) noexcept(std::is_nothrow_copy_constructible_v<value_type>)
        -> value_type&;
    constexpr auto push_back(value_type&& value) noexcept(std::is_nothrow_move_constructible_v<value_type>)
        -> value_type&;
    constexpr auto pop_back() noexcept(std::is_nothrow_destructible_v<value_type>) -> void;
    constexpr auto clear() noexcept(std::is_nothrow_destructible_v<value_type>) -> void;

private:
    // @speed: We could determine what the smallest unsigned integer type capable of storing the values between 0 and
    // Capacity is and use it instead of end pointer (or maybe even forgo it completely if Capacity is 0).
    pointer _end = nullptr;
    // @refactor: Use StaticBuffer instead.
    alignas(value_type) std::array<byte, sizeof(value_type) * Capacity> _data;
};

} // namespace zth

#include "vector.inl"
