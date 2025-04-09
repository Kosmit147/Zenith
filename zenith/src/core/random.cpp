#include "zenith/core/random.hpp"

#include "zenith/log/logger.hpp"

namespace zth {

std::random_device Random::_random_device;
std::mt19937_64 Random::_generator;

auto Random::init() -> Result<void, String>
{
    ZTH_INTERNAL_TRACE("Initializing random number generator...");
    _generator.seed(_random_device());
    ZTH_INTERNAL_TRACE("Random number generator initialized.");
    return {};
}

auto Random::shut_down() -> void
{
    ZTH_INTERNAL_TRACE("Random number generator shut down.");
}

template auto Random::get_int<u16>(u16, u16) -> u16;
template auto Random::get_int<i16>(i16, i16) -> i16;
template auto Random::get_int<u32>(u32, u32) -> u32;
template auto Random::get_int<i32>(i32, i32) -> i32;
template auto Random::get_int<u64>(u64, u64) -> u64;
template auto Random::get_int<i64>(i64, i64) -> i64;

template auto Random::get_float<float>(float, float) -> float;
template auto Random::get_float<double>(double, double) -> double;

template auto Random::angle<float>() -> float;
template auto Random::angle<double>() -> double;

template auto Random::point_on_sphere<float>(float) -> glm::vec<3, float>;
template auto Random::point_inside_sphere<float>(float) -> glm::vec<3, float>;
template auto Random::point_on_sphere<double>(double) -> glm::vec<3, double>;
template auto Random::point_inside_sphere<double>(double) -> glm::vec<3, double>;

template auto Random::point_on_circle<float>(float) -> glm::vec<2, float>;
template auto Random::point_inside_circle<float>(float) -> glm::vec<2, float>;
template auto Random::point_on_circle<double>(double) -> glm::vec<2, double>;
template auto Random::point_inside_circle<double>(double) -> glm::vec<2, double>;

template auto Random::perlin_noise<2, float>(glm::vec<2, float>) -> float;
template auto Random::perlin_noise<3, float>(glm::vec<3, float>) -> float;
template auto Random::perlin_noise<4, float>(glm::vec<4, float>) -> float;
template auto Random::perlin_noise<2, double>(glm::vec<2, double>) -> double;
template auto Random::perlin_noise<3, double>(glm::vec<3, double>) -> double;
template auto Random::perlin_noise<4, double>(glm::vec<4, double>) -> double;

template auto Random::normal<float>(float, float) -> float;
template auto Random::normal<double>(double, double) -> double;

template auto Random::linear<float>(float, float) -> float;
template auto Random::linear<double>(double, double) -> double;

} // namespace zth
