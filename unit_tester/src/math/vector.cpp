#include <zenith/math/vector.hpp>

TEST_CASE("Extracting min and max component from a vector works", "[Vec]")
{
    {
        glm::vec3 vec{ 0.0f, 2.0f, 1.0f };

        REQUIRE(zth::math::min_component(vec) == 0.0f);
        REQUIRE(zth::math::max_component(vec) == 2.0f);
        REQUIRE(zth::math::min_max_component(vec) == std::pair{ 0.0f, 2.0f });
    }

    {
        glm::ivec4 vec{ 4, 5, 6, 7 };

        REQUIRE(zth::math::min_component(vec) == 4);
        REQUIRE(zth::math::max_component(vec) == 7);
        REQUIRE(zth::math::min_max_component(vec) == std::pair{ 4, 7 });
    }

    {
        glm::uvec4 vec{ 4, 7, 4, 5 };

        REQUIRE(zth::math::min_component(vec) == 4);
        REQUIRE(zth::math::max_component(vec) == 7);
        REQUIRE(zth::math::min_max_component(vec) == std::pair{ 4, 7 });
    }
}
