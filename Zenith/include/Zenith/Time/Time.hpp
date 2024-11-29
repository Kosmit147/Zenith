#pragma once

#include <type_traits>

#include "Timer.hpp"

namespace zth {

class Time
{
public:
    Time() = delete;

    static auto init() -> void;
    static auto on_update() -> void;
    static auto shut_down() -> void;

    // clang-format off
    template<std::floating_point T = double> [[nodiscard]] static auto time() -> T
    {
        return static_cast<T>(_time);
    }

    template<std::floating_point T = double> [[nodiscard]] static auto delta_time() -> T
    {
        return static_cast<T>(_delta_time);
    }
    // clang-format on

private:
    static inline double _time = 0.0;
    static inline double _delta_time = 0.0;
};

} // namespace zth
