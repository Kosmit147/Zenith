#pragma once

#include <glm/gtc/noise.hpp>
#include <glm/gtc/random.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <concepts>
#include <numbers>
#include <random>

#include "zenith/core/assert.hpp"
#include "zenith/core/typedefs.hpp"
#include "zenith/math/quaternion.hpp"
#include "zenith/stl/string.hpp"
#include "zenith/util/result.hpp"

namespace zth {

// @speed: Use a faster random number generator.

class Random
{
public:
    Random() = delete;

    [[nodiscard]] static auto init() -> Result<void, String>;
    static auto shut_down() -> void;

    // Generates a random integer in range [min; max] (inclusive).
    template<std::integral T = int> [[nodiscard]] static auto get_int(T min, T max) -> T
    {
        ZTH_ASSERT(min <= max);
        std::uniform_int_distribution dist{ min, max };
        return dist(_generator);
    }

    // Generates a random floating-point number in range [min; max).
    template<std::floating_point T = float>
    [[nodiscard]] static auto get_float(T min = static_cast<T>(0.0), T max = static_cast<T>(1.0)) -> T
    {
        ZTH_ASSERT(min <= max);
        std::uniform_real_distribution dist{ min, max };
        return dist(_generator);
    }

    // Generates a random floating-point number in range [0 rad; 6.28319 rad).
    template<std::floating_point T = float> [[nodiscard]] static auto angle() -> T
    {
        static constexpr auto full_angle = std::numbers::pi_v<T> * static_cast<T>(2);
        return get_float<T>(static_cast<T>(0), full_angle);
    }

    static auto rgb_color() -> glm::vec3;
    static auto rgba_color() -> glm::vec4;

    static auto rgb8_color() -> glm::vec<3, u8>;
    static auto rgba8_color() -> glm::vec<4, u8>;

    template<std::floating_point T = float>
    [[nodiscard]] static auto point_on_sphere(T radius = static_cast<T>(1.0)) -> glm::vec<3, T>
    {
        return glm::sphericalRand(radius);
    }

    template<std::floating_point T = float>
    [[nodiscard]] static auto point_inside_sphere(T radius = static_cast<T>(1.0)) -> glm::vec<3, T>
    {
        return glm::ballRand(radius);
    }

    template<std::floating_point T = float>
    [[nodiscard]] static auto point_on_circle(T radius = static_cast<T>(1.0)) -> glm::vec<2, T>
    {
        return glm::circularRand(radius);
    }

    template<std::floating_point T = float>
    [[nodiscard]] static auto point_inside_circle(T radius = static_cast<T>(1.0)) -> glm::vec<2, T>
    {
        return glm::diskRand(radius);
    }

    [[nodiscard]] static auto rotation() -> math::AngleAxis
    {
        return math::AngleAxis{
            .angle = angle(),
            .axis = point_on_sphere(),
        };
    }

    template<glm::length_t Size, std::floating_point T = float>
        requires(Size > 1)
    [[nodiscard]] static auto perlin_noise(glm::vec<Size, T> position) -> T
    {
        return glm::perlin(position);
    }

    template<std::floating_point T = float>
    [[nodiscard]] static auto normal(T mean = static_cast<T>(0.0), T deviation = static_cast<T>(1.0)) -> T
    {
        return glm::gaussRand(mean, deviation);
    }

    template<std::floating_point T = float>
    [[nodiscard]] static auto linear(T min = static_cast<T>(0.0), T max = static_cast<T>(1.0)) -> T
    {
        ZTH_ASSERT(min <= max);
        return glm::linearRand(min, max);
    }

private:
    static std::random_device _random_device;
    static std::mt19937_64 _generator;
};

extern template auto Random::get_int<u16>(u16, u16) -> u16;
extern template auto Random::get_int<i16>(i16, i16) -> i16;
extern template auto Random::get_int<u32>(u32, u32) -> u32;
extern template auto Random::get_int<i32>(i32, i32) -> i32;
extern template auto Random::get_int<u64>(u64, u64) -> u64;
extern template auto Random::get_int<i64>(i64, i64) -> i64;

extern template auto Random::get_float<float>(float, float) -> float;
extern template auto Random::get_float<double>(double, double) -> double;

extern template auto Random::angle<float>() -> float;
extern template auto Random::angle<double>() -> double;

extern template auto Random::point_on_sphere<float>(float) -> glm::vec<3, float>;
extern template auto Random::point_inside_sphere<float>(float) -> glm::vec<3, float>;
extern template auto Random::point_on_sphere<double>(double) -> glm::vec<3, double>;
extern template auto Random::point_inside_sphere<double>(double) -> glm::vec<3, double>;

extern template auto Random::point_on_circle<float>(float) -> glm::vec<2, float>;
extern template auto Random::point_inside_circle<float>(float) -> glm::vec<2, float>;
extern template auto Random::point_on_circle<double>(double) -> glm::vec<2, double>;
extern template auto Random::point_inside_circle<double>(double) -> glm::vec<2, double>;

extern template auto Random::perlin_noise<2, float>(glm::vec<2, float>) -> float;
extern template auto Random::perlin_noise<3, float>(glm::vec<3, float>) -> float;
extern template auto Random::perlin_noise<4, float>(glm::vec<4, float>) -> float;
extern template auto Random::perlin_noise<2, double>(glm::vec<2, double>) -> double;
extern template auto Random::perlin_noise<3, double>(glm::vec<3, double>) -> double;
extern template auto Random::perlin_noise<4, double>(glm::vec<4, double>) -> double;

extern template auto Random::normal<float>(float, float) -> float;
extern template auto Random::normal<double>(double, double) -> double;

extern template auto Random::linear<float>(float, float) -> float;
extern template auto Random::linear<double>(double, double) -> double;

} // namespace zth
