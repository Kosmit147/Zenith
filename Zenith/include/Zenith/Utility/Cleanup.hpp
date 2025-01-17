#pragma once

#include <concepts>
#include <utility>

#include <Zenith/Utility/Macros.hpp>

namespace zth {

template<std::invocable Func> class Cleanup
{
public:
    constexpr explicit Cleanup(const Func& func) : _func(func) {}
    constexpr explicit Cleanup(Func&& func) : _func(std::move(func)) {}
    ZTH_NO_COPY_NO_MOVE(Cleanup)
    constexpr ~Cleanup() { release(); }

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
