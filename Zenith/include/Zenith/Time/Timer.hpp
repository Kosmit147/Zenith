#pragma once

#include <chrono>

#include "Zenith/Core/Typedefs.hpp"

namespace zth {

class Timer
{
public:
    Timer() { reset(); }

    auto reset() -> void { _start = std::chrono::high_resolution_clock::now(); }

    [[nodiscard]] auto elapsed_ns() const -> usize
    {
        auto ns =
            std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - _start);
        return static_cast<usize>(ns.count());
    }

    [[nodiscard]] auto elapsed_ms() const -> double { return static_cast<double>(elapsed_ns()) / (1000.0 * 1000.0); }
    [[nodiscard]] auto elapsed_s() const -> double { return elapsed_ms() / 1000.0; }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> _start;
};

} // namespace zth
