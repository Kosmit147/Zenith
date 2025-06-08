#pragma once

#include <magic_enum/magic_enum.hpp>

#include <limits>
#include <type_traits>
#include <utility>

#include "zenith/core/typedefs.hpp"
#include "zenith/log/format.hpp"

namespace zth::meta {

// clang-format off

// Smallest unsigned integer that can represent values in range <0, N>.
template<usize N> using SmallestUint = 
    std::conditional_t<N <= std::numeric_limits<u8>::max(), u8,
    std::conditional_t<N <= std::numeric_limits<u16>::max(), u16,
    std::conditional_t<N <= std::numeric_limits<u32>::max(), u32,
    std::conditional_t<N <= std::numeric_limits<u64>::max(), u64,
    usize>>>>;

// clang-format on

template<usize> struct Anything
{
    template<typename T> constexpr operator T() const { return T{}; }
};

template<typename T, usize... Sequence>
concept ConstructibleFromNInitializers = requires { T{ Anything<Sequence>{}... }; };

template<typename T, usize N> [[nodiscard]] constexpr auto constructible_from_n_initializers() -> bool
{
    static constexpr auto unpack = []<usize... Sequence>(std::index_sequence<Sequence...>) {
        return ConstructibleFromNInitializers<T, Sequence...>;
    };

    return unpack(std::make_index_sequence<N>{});
}

// Returns the number of fields in a struct.
template<typename T, usize N = 0> [[nodiscard]] constexpr auto struct_arity() -> usize
{
    static constexpr auto constructible = constructible_from_n_initializers<T, N>();

    if constexpr (!constructible)
        return N - 1;
    else
        return struct_arity<T, N + 1>();
}

template<typename T> constexpr inline auto reflected_enum_tag = false;

template<typename T>
concept EnumWithDefinedRange = std::is_enum_v<std::remove_cvref_t<T>> && requires {
    { std::remove_cvref_t<T>::MinEnumValue };
    { std::remove_cvref_t<T>::MaxEnumValue };
};

template<typename T>
concept ReflectedEnum = EnumWithDefinedRange<T> && reflected_enum_tag<std::remove_cvref_t<T>>;

// Enum must have MinEnumValue and MaxEnumValue aliases defined on it.
#define ZTH_DERIVE_REFLECTED_ENUM_RANGE(enum_type)                                                                     \
    template<> struct ::magic_enum::customize::enum_range<enum_type>                                                   \
    {                                                                                                                  \
        static_assert(::zth::meta::ReflectedEnum<enum_type>);                                                          \
        static constexpr int min = static_cast<int>(std::to_underlying(enum_type::MinEnumValue));                      \
        static constexpr int max = static_cast<int>(std::to_underlying(enum_type::MaxEnumValue));                      \
        static_assert(max - min < std::numeric_limits<::zth::u16>::max(),                                              \
                      "max - min must be less than UINT16_MAX due to magic enum's limitations");                       \
    };

#define ZTH_DERIVE_REFLECTED_ENUM_FORMATTER(enum_type)                                                                 \
    ZTH_DEFINE_FORMATTER(enum_type, enum_value)                                                                        \
    {                                                                                                                  \
        static_assert(::zth::meta::ReflectedEnum<enum_type>);                                                          \
        return ZTH_FORMAT_OUT("{}", ::zth::meta::enum_name(enum_value));                                               \
    }

// Must be used together with ZTH_DEFINE_REFLECTED_ENUM macro. Enum must have MinEnumValue and MaxEnumValue aliases
// defined on it.
#define ZTH_DECLARE_REFLECTED_ENUM(enum_type)                                                                          \
    static_assert(::zth::meta::EnumWithDefinedRange<enum_type>);                                                       \
    template<> constexpr inline auto ::zth::meta::reflected_enum_tag<enum_type> = true;                                \
    ZTH_DERIVE_REFLECTED_ENUM_RANGE(enum_type);                                                                        \
    ZTH_DECLARE_FORMATTER(enum_type)

// Must be used only once in an implementation file.
#define ZTH_DEFINE_REFLECTED_ENUM(enum_type) ZTH_DERIVE_REFLECTED_ENUM_FORMATTER(enum_type)

[[nodiscard]] constexpr auto enum_name(ReflectedEnum auto&& e) -> decltype(auto)
{
    return magic_enum::enum_name(e);
}

template<ReflectedEnum E> [[nodiscard]] constexpr auto enum_values() -> decltype(auto)
{
    return magic_enum::enum_values<E>();
}

template<ReflectedEnum E> [[nodiscard]] constexpr auto enum_type_name() -> decltype(auto)
{
    return magic_enum::enum_type_name<E>();
}

} // namespace zth::meta
