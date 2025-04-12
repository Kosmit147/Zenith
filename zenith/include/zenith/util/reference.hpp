#pragma once

#include <functional>
#include <type_traits>

namespace zth {

template<typename T> using Reference = std::reference_wrapper<T>;

[[nodiscard]] constexpr auto make_reference(auto&& value) -> decltype(auto)
{
    return std::ref(std::forward<decltype(value)>(value));
}

[[nodiscard]] constexpr auto make_const_reference(auto&& value) -> decltype(auto)
{
    return std::cref(std::forward<decltype(value)>(value));
}

// Transforms:
// T -> T
// T& -> Reference<T>
// const T& -> Reference<const T>
template<typename T>
    requires(!std::is_rvalue_reference_v<T>)
using WrapReference = std::conditional_t<std::is_lvalue_reference_v<T>, Reference<std::remove_reference_t<T>>, T>;

} // namespace zth
