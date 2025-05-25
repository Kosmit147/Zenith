#pragma once

#include <concepts>

#include "zenith/stl/string.hpp"
#include "zenith/util/result.hpp"

namespace zth {

struct TimeSpec
{
    double fixed_time_step = 1.0 / 60.0;  // 60 time steps per second.
    double delta_time_limit = 1.0 / 30.0; // 1 second divided by 30 fps.
};

class Time
{
public:
    Time() = delete;

    [[nodiscard]] static auto init(const TimeSpec& spec) -> Result<void, String>;
    static auto start_frame() -> void;
    static auto shut_down() -> void;

    template<std::floating_point T = double> [[nodiscard]] static auto time() -> T { return static_cast<T>(_time); }

    template<std::floating_point T = double> [[nodiscard]] static auto delta_time() -> T
    {
        return static_cast<T>(_delta_time);
    }

    template<std::floating_point T = double> [[nodiscard]] static auto delta_time_limit() -> T
    {
        return static_cast<T>(_delta_time_limit);
    }

    template<std::floating_point T = double> [[nodiscard]] static auto fixed_time_step() -> T
    {
        return static_cast<T>(_fixed_time_step);
    }

private:
    static inline double _time = 0.0;
    static inline double _delta_time = 0.0;
    static inline double _delta_time_limit = 0.0;
    static inline double _fixed_time_step = 0.0;
};

} // namespace zth
