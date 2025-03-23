#pragma once

#include <utility>

#include "zenith/core/typedefs.hpp"

namespace zth::meta {

template<usize N> struct Anything
{
    template<typename T> constexpr operator T() const { return T{}; }
};

template<typename T, usize... Ints>
concept ConstructibleFromNInitializers = requires { T{ Anything<Ints>{}... }; };

template<typename T, usize N> [[nodiscard]] constexpr auto constructible_from_n_initializers() -> bool
{
    constexpr auto unpack = [&]<usize... Ints>(std::index_sequence<Ints...>) {
        return ConstructibleFromNInitializers<T, Ints...>;
    };

    return unpack(std::make_index_sequence<N>{});
}

// returns the number of fields in a struct
template<typename T, usize N = 0> [[nodiscard]] constexpr auto struct_arity() -> usize
{
    constexpr auto constructible = constructible_from_n_initializers<T, N>();

    if constexpr (!constructible)
        return N - 1;
    else
        return struct_arity<T, N + 1>();
}

} // namespace zth::meta
