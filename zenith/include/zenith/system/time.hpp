#pragma once

#include <concepts>

#include "zenith/stl/string.hpp"
#include "zenith/util/result.hpp"

namespace zth {

class Time
{
public:
    static constexpr auto delta_time_limit = 1.0 / 30.0; // 1 second divided by 30 fps

public:
    Time() = delete;

    [[nodiscard]] static auto init() -> Result<void, String>;
    static auto start_frame() -> void;
    static auto shut_down() -> void;

    template<std::floating_point T = double> [[nodiscard]] static auto time() -> T { return static_cast<T>(_time); }

    template<std::floating_point T = double> [[nodiscard]] static auto delta_time() -> T
    {
        return static_cast<T>(_delta_time);
    }

private:
    static inline double _time = 0.0;
    static inline double _delta_time = 0.0;
};

} // namespace zth
