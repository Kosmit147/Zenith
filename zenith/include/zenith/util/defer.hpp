#pragma once

#include <concepts>
#include <utility>

#include "zenith/util/macros.hpp"

namespace zth {

template<std::invocable Func> class Defer
{
public:
    constexpr explicit Defer(const Func& func) : _func(func) {}
    constexpr explicit Defer(Func&& func) : _func(std::move(func)) {}
    ZTH_NO_COPY_NO_MOVE(Defer)
    constexpr ~Defer() { release(); }

    constexpr auto dismiss() noexcept -> void { _dismissed = true; }

    constexpr auto release() -> void
    {
        if (!_dismissed)
            _func();

        _dismissed = true;
    }

private:
    Func _func;
    bool _dismissed = false;
};

} // namespace zth
