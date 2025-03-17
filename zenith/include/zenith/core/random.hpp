#pragma once

#include <concepts>
#include <numbers>
#include <random>

#include "zenith/core/assert.hpp"
#include "zenith/core/typedefs.hpp"

namespace zth {

// @speed: Use a faster random number generator.

class Random
{
public:
    Random() = delete;

    static auto init() -> void;
    static auto shut_down() -> void;

    // Generates a random integer in range [min; max] (inclusive).
    template<std::integral T = int> [[nodiscard]] static auto random_int(T min, T max) -> T
    {
        ZTH_ASSERT(min <= max);
        std::uniform_int_distribution dist(min, max);
        return dist(_generator);
    }

    // Generates a random floating-point number in range [min; max).
    template<std::floating_point T = float> [[nodiscard]] static auto random_float(T min = 0.0, T max = 1.0) -> T
    {
        ZTH_ASSERT(min <= max);
        std::uniform_real_distribution dist(min, max);
        return dist(_generator);
    }

    // Generates a random floating-point number in range [0 rad; 6.28319 rad).
    template<std::floating_point T = float> [[nodiscard]] static auto random_angle() -> T
    {
        constexpr auto full_angle = std::numbers::pi_v<T> * 2;
        return random_float<T>(0, full_angle);
    }

private:
    static std::random_device _random_device;
    static std::mt19937_64 _generator;
};

extern template auto Random::random_int<u16>(u16, u16) -> u16;
extern template auto Random::random_int<i16>(i16, i16) -> i16;
extern template auto Random::random_int<u32>(u32, u32) -> u32;
extern template auto Random::random_int<i32>(i32, i32) -> i32;
extern template auto Random::random_int<u64>(u64, u64) -> u64;
extern template auto Random::random_int<i64>(i64, i64) -> i64;

extern template auto Random::random_float<float>(float, float) -> float;
extern template auto Random::random_float<double>(double, double) -> double;

extern template auto Random::random_angle<float>() -> float;
extern template auto Random::random_angle<double>() -> double;

} // namespace zth
