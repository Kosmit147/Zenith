#pragma once

#include <initializer_list>
#include <optional>
#include <utility>

namespace zth {

template<typename T> using Optional = std::optional<T>;
using Nil = std::nullopt_t;
constexpr auto nil = std::nullopt;

[[nodiscard]] constexpr auto make_optional(auto&& value) -> decltype(auto)
{
    return std::make_optional(std::forward<decltype(value)>(value));
}

template<typename T> [[nodiscard]] constexpr auto make_optional(auto&&... args) -> decltype(auto)
{
    return std::make_optional<T>(std::forward<decltype(args)>(args)...);
}

template<typename T, typename Elem>
[[nodiscard]] constexpr auto make_optional(std::initializer_list<Elem> values, auto&&... args) -> decltype(auto)
{
    return std::make_optional<T>(values, std::forward<decltype(args)>(args)...);
}

} // namespace zth
