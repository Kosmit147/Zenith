#pragma once

#include <concepts>
#include <initializer_list>
#include <type_traits>
#include <vector>

#include "zenith/core/typedefs.hpp"
#include "zenith/memory/alloc.hpp"
#include "zenith/memory/buffer.hpp"
#include "zenith/stl/range.hpp"
#include "zenith/util/optional.hpp"
#include "zenith/util/reference.hpp"

namespace zth {

// @todo: Implement iterators.
// @todo: Implement reverse iterators (rbegin, rend).

// A resizable array.
// @todo: Implement Vector.
template<std::movable T, memory::Allocator A = memory::DefaultAllocator<T>> using Vector = std::vector<T, A>;

// A vector which stores its data on the stack, if the number of stored objects is not greater than Capacity, or on the
// heap otherwise.
// Not currently implemented.
// @todo: Implement SmallVector.
template<std::movable T, usize StackCapacity, memory::Allocator A = memory::DefaultAllocator<T>>
using SmallVector = Vector<T, A>;

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
    [[nodiscard]] constexpr auto data() noexcept -> pointer;
    [[nodiscard]] constexpr auto data() const noexcept -> const_pointer;
    [[nodiscard]] constexpr auto begin(this auto&& self) noexcept -> decltype(auto);
    [[nodiscard]] constexpr auto end(this auto&& self) noexcept -> decltype(auto);
    [[nodiscard]] constexpr auto cbegin() const noexcept -> const_iterator;
    [[nodiscard]] constexpr auto cend() const noexcept -> const_iterator;
    [[nodiscard]] constexpr auto size() const noexcept -> size_type;
    [[nodiscard]] static constexpr auto capacity() noexcept -> size_type;

    // --- InPlaceVector implementation
    constexpr auto emplace_back(auto&&... args) noexcept(std::is_nothrow_constructible_v<value_type, decltype(args)...>)
        -> value_type&;
    constexpr auto push_back(const value_type& value) noexcept(std::is_nothrow_copy_constructible_v<value_type>)
        -> value_type&;
    constexpr auto push_back(value_type&& value) noexcept(std::is_nothrow_move_constructible_v<value_type>)
        -> value_type&;
    constexpr auto pop_back() noexcept(std::is_nothrow_destructible_v<value_type>) -> void;
    constexpr auto clear() noexcept(std::is_nothrow_destructible_v<value_type>) -> void;

    constexpr auto try_emplace_back(auto&&... args)
        noexcept(std::is_nothrow_constructible_v<value_type, decltype(args)...>) -> Optional<Reference<value_type>>;
    constexpr auto try_push_back(const value_type& value) noexcept(std::is_nothrow_copy_constructible_v<value_type>)
        -> Optional<Reference<value_type>>;
    constexpr auto try_push_back(value_type&& value) noexcept(std::is_nothrow_move_constructible_v<value_type>)
        -> Optional<Reference<value_type>>;

    friend constexpr auto swap(InPlaceVector& first, InPlaceVector& second)
        noexcept(std::is_nothrow_swappable_v<value_type> && std::is_nothrow_move_constructible_v<value_type>) -> void
    {
        if (first.size() < second.size())
            swap_impl(first, second);
        else
            swap_impl(second, first);
    }

private:
    // @speed: We could determine what the smallest unsigned integer type capable of storing the values between 0 and
    // Capacity is and use it instead of end pointer (or maybe even forgo it completely if Capacity is 0).
    pointer _end = nullptr;
    memory::StaticBuffer<sizeof(value_type) * Capacity, alignof(value_type)> _data;

private:
    static constexpr auto swap_impl(InPlaceVector& smaller, InPlaceVector& bigger)
        noexcept(std::is_nothrow_swappable_v<value_type> && std::is_nothrow_move_constructible_v<value_type>) -> void;
};

} // namespace zth

#include "vector.inl"
