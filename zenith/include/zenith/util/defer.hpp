#pragma once

#include <concepts>
#include <functional>
#include <type_traits>
#include <utility>

#include "zenith/util/macros.hpp"

namespace zth {

template<std::invocable Func> class Defer
{
public:
    constexpr explicit Defer(const Func& func) noexcept(std::is_nothrow_copy_constructible_v<Func>) : _func(func) {}
    constexpr explicit Defer(Func&& func) noexcept(std::is_nothrow_move_constructible_v<Func>) : _func(std::move(func))
    {}

    ZTH_NO_COPY_NO_MOVE(Defer)

    constexpr ~Defer() { release(); }

    constexpr auto dismiss() noexcept -> void { _dismissed = true; }

    constexpr auto release() noexcept(std::is_nothrow_invocable_v<Func>) -> void
    {
        if (!_dismissed)
            std::invoke(_func);

        dismiss();
    }

private:
    Func _func;
    bool _dismissed = false;
};

} // namespace zth
