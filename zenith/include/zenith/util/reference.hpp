#pragma once

#include <functional>
#include <type_traits>

namespace zth {

template<typename T> using Reference = std::reference_wrapper<T>;

template<typename T>
    requires(!std::is_rvalue_reference_v<T>)
using WrapReference = std::conditional_t<std::is_lvalue_reference_v<T>, Reference<std::remove_reference_t<T>>, T>;

} // namespace zth
