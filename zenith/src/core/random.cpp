#include "zenith/core/random.hpp"

#include "zenith/log/logger.hpp"

namespace zth {

std::random_device Random::_random_device;
std::mt19937_64 Random::_generator;

auto Random::init() -> void
{
    ZTH_CORE_INFO("Initializing random number generator...");
    _generator.seed(_random_device());
    ZTH_CORE_INFO("Random number generator initialized.");
}

auto Random::shut_down() -> void
{
    ZTH_CORE_INFO("Random number generator shut down.");
}

template auto Random::random_int<u16>(u16, u16) -> u16;
template auto Random::random_int<i16>(i16, i16) -> i16;
template auto Random::random_int<u32>(u32, u32) -> u32;
template auto Random::random_int<i32>(i32, i32) -> i32;
template auto Random::random_int<u64>(u64, u64) -> u64;
template auto Random::random_int<i64>(i64, i64) -> i64;

template auto Random::random_float<float>(float, float) -> float;
template auto Random::random_float<double>(double, double) -> double;

template auto Random::random_angle<float>() -> float;
template auto Random::random_angle<double>() -> double;

} // namespace zth
