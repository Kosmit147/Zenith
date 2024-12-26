#pragma once

#include <array>
#include <concepts>
#include <cstddef>
#include <initializer_list>
#include <type_traits>

#include "Zenith/Core/Typedefs.hpp"

namespace zth {

template<std::movable T, usize Size> class InPlaceVector
{
public:
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

    // --- Element access
    [[nodiscard]] constexpr auto at(usize index) -> T&;
    [[nodiscard]] constexpr auto at(usize index) const -> const T&;

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
    usize _size = 0;
    alignas(T) std::array<std::byte, sizeof(T) * Size> _data{};
};

} // namespace zth

#include "InPlaceVector.inl"
