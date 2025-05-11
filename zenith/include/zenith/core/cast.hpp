#pragma once

#include <concepts>
#include <limits>

namespace zth {

template<std::unsigned_integral IntoUnsigned, std::signed_integral FromSigned>
[[nodiscard]] constexpr auto can_cast(FromSigned n) -> bool
{
    using limits = std::numeric_limits<IntoUnsigned>;

    if (n < static_cast<FromSigned>(0))
        return false;

    if constexpr (sizeof(IntoUnsigned) < sizeof(FromSigned))
    {
        if (n > static_cast<FromSigned>(limits::max()))
            return false;
    }

    return true;
}

template<std::signed_integral IntoSigned, std::unsigned_integral FromUnsigned>
[[nodiscard]] constexpr auto can_cast(FromUnsigned n) -> bool
{
    using limits = std::numeric_limits<IntoSigned>;

    if constexpr (sizeof(IntoSigned) <= sizeof(FromUnsigned))
    {
        if (n > static_cast<FromUnsigned>(limits::max()))
            return false;
    }

    return true;
}

template<std::signed_integral IntoSigned, std::signed_integral FromSigned>
[[nodiscard]] constexpr auto can_cast(FromSigned n) -> bool
{
    using limits = std::numeric_limits<IntoSigned>;

    if constexpr (sizeof(IntoSigned) < sizeof(FromSigned))
    {
        if (n > static_cast<FromSigned>(limits::max()) || n < static_cast<FromSigned>(limits::min()))
            return false;
    }

    return true;
}

template<std::unsigned_integral IntoUnsigned, std::unsigned_integral FromUnsigned>
[[nodiscard]] constexpr auto can_cast(FromUnsigned n) -> bool
{
    using limits = std::numeric_limits<IntoUnsigned>;

    if constexpr (sizeof(IntoUnsigned) < sizeof(FromUnsigned))
    {
        if (n > static_cast<FromUnsigned>(limits::max()))
            return false;
    }

    return true;
}

template<std::unsigned_integral IntoUnsigned, std::signed_integral FromSigned>
[[nodiscard]] constexpr auto clamping_cast(FromSigned n) -> IntoUnsigned
{
    using limits = std::numeric_limits<IntoUnsigned>;

    if (n < static_cast<FromSigned>(0))
        return static_cast<IntoUnsigned>(0);

    if constexpr (sizeof(IntoUnsigned) < sizeof(FromSigned))
    {
        if (n > static_cast<FromSigned>(limits::max()))
            return limits::max();
    }

    return static_cast<IntoUnsigned>(n);
}

template<std::signed_integral IntoSigned, std::unsigned_integral FromUnsigned>
[[nodiscard]] constexpr auto clamping_cast(FromUnsigned n) -> IntoSigned
{
    using limits = std::numeric_limits<IntoSigned>;

    if constexpr (sizeof(IntoSigned) <= sizeof(FromUnsigned))
    {
        if (n > static_cast<FromUnsigned>(limits::max()))
            return limits::max();
    }

    return static_cast<IntoSigned>(n);
}

template<std::signed_integral IntoSigned, std::signed_integral FromSigned>
[[nodiscard]] constexpr auto clamping_cast(FromSigned n) -> IntoSigned
{
    using limits = std::numeric_limits<IntoSigned>;

    if constexpr (sizeof(IntoSigned) < sizeof(FromSigned))
    {
        if (n > static_cast<FromSigned>(limits::max()))
            return limits::max();
        else if (n < static_cast<FromSigned>(limits::min()))
            return limits::min();
    }

    return static_cast<IntoSigned>(n);
}

template<std::unsigned_integral IntoUnsigned, std::unsigned_integral FromUnsigned>
[[nodiscard]] constexpr auto clamping_cast(FromUnsigned n) -> IntoUnsigned
{
    using limits = std::numeric_limits<IntoUnsigned>;

    if constexpr (sizeof(IntoUnsigned) < sizeof(FromUnsigned))
    {
        if (n > static_cast<FromUnsigned>(limits::max()))
            return limits::max();
    }

    return static_cast<IntoUnsigned>(n);
}

} // namespace zth
