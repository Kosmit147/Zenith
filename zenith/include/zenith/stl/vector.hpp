#pragma once

#include <array>
#include <concepts>
#include <functional>
#include <initializer_list>
#include <optional>
#include <type_traits>

// @cleanup: Remove this include if it's not used once SmallVector is implemented.
#include <vector>

#include "zenith/core/typedefs.hpp"

namespace zth {

// @refactor: Implement iterators.

// A vector which stores its data on the stack, if the number of stored objects is not greater than Capacity, or on the
// heap otherwise.
// Not currently implemented.
// @todo: Implement SmallVector.
template<typename T, usize Capacity> using SmallVector = std::vector<T>;

// A vector which stores its data on the stack. The amount of stored objects cannot go above the specified Capacity.
// @refactor: Replace with std::inplace_vector once we're on C++26.
template<std::movable T, usize Capacity> class InPlaceVector
{
public:
    using TRef = std::reference_wrapper<T>;
    using ConstTRef = std::reference_wrapper<const T>;

    constexpr InPlaceVector() noexcept = default;
    constexpr explicit InPlaceVector(usize count) noexcept(std::is_nothrow_default_constructible_v<T>);
    constexpr InPlaceVector(usize count, const T& value) noexcept(std::is_nothrow_copy_constructible_v<T>);
    constexpr InPlaceVector(std::initializer_list<T> values) noexcept(std::is_nothrow_copy_constructible_v<T>);

    constexpr InPlaceVector(const InPlaceVector& other) noexcept(std::is_nothrow_copy_constructible_v<T>);
    constexpr auto operator=(const InPlaceVector& other)
        noexcept(std::is_nothrow_copy_constructible_v<T> && std::is_nothrow_destructible_v<T>) -> InPlaceVector&;

    constexpr InPlaceVector(InPlaceVector&& other) noexcept(std::is_nothrow_move_constructible_v<T>);
    constexpr auto operator=(InPlaceVector&& other)
        noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_destructible_v<T>) -> InPlaceVector&;

    constexpr ~InPlaceVector() noexcept(std::is_nothrow_destructible_v<T>);

    // --- Data access
    [[nodiscard]] constexpr auto at(usize index) noexcept -> std::optional<TRef>;
    [[nodiscard]] constexpr auto at(usize index) const noexcept -> std::optional<ConstTRef>;

    [[nodiscard]] constexpr auto operator[](usize index) noexcept -> T&;
    [[nodiscard]] constexpr auto operator[](usize index) const noexcept -> const T&;

    [[nodiscard]] constexpr auto front() noexcept -> T&;
    [[nodiscard]] constexpr auto front() const noexcept -> const T&;

    [[nodiscard]] constexpr auto back() noexcept -> T&;
    [[nodiscard]] constexpr auto back() const noexcept -> const T&;

    [[nodiscard]] constexpr auto data() noexcept -> T*;
    [[nodiscard]] constexpr auto data() const noexcept -> const T*;

    // --- Iterators
    [[nodiscard]] constexpr auto begin() noexcept -> T*;
    [[nodiscard]] constexpr auto begin() const noexcept -> const T*;
    [[nodiscard]] constexpr auto cbegin() noexcept -> T*;
    [[nodiscard]] constexpr auto cbegin() const noexcept -> const T*;

    [[nodiscard]] constexpr auto end() noexcept -> T*;
    [[nodiscard]] constexpr auto end() const noexcept -> const T*;
    [[nodiscard]] constexpr auto cend() noexcept -> T*;
    [[nodiscard]] constexpr auto cend() const noexcept -> const T*;

    // --- Size and capacity
    [[nodiscard]] constexpr auto empty() const noexcept -> bool;
    [[nodiscard]] constexpr auto size() const noexcept -> usize;
    [[nodiscard]] static constexpr auto max_size() noexcept -> usize;
    [[nodiscard]] static constexpr auto capacity() noexcept -> usize;

    // --- Modifiers
    template<typename... Args>
    constexpr auto emplace_back(Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>) -> T&;
    constexpr auto push_back(const T& value) noexcept(std::is_nothrow_copy_constructible_v<T>) -> T&;
    constexpr auto push_back(T&& value) noexcept(std::is_nothrow_move_constructible_v<T>) -> T&;
    constexpr auto pop_back() noexcept(std::is_nothrow_destructible_v<T>) -> void;
    constexpr auto clear() noexcept(std::is_nothrow_destructible_v<T>) -> void;

private:
    // @speed: We could determine what the smallest unsigned integer type capable of storing the values between 0 and
    // Capacity is and use it instead of usize (or maybe even forgo it completely if Capacity is 0).
    usize _size = 0;
    alignas(T) std::array<byte, sizeof(T) * Capacity> _data;
};

} // namespace zth

#include "vector.inl"
