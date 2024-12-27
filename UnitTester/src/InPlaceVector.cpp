#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_quantifiers.hpp>

#include <Zenith/Utility/InPlaceVector.hpp>

#include "Matchers.hpp"

using Catch::Matchers::AllMatch;

TEST_CASE("InPlaceVector constructors work", "[InPlaceVector]")
{
    SECTION("default constructor works")
    {
        zth::InPlaceVector<int, 10> int_vec{};

        REQUIRE(int_vec.size() == 0);
        REQUIRE(int_vec.capacity() == 10);
        REQUIRE(int_vec.begin() == int_vec.end());
    }

    SECTION("constructor which fills vec with n default values works")
    {
        zth::InPlaceVector<std::string, 3> string_vec{ 2 };
        std::string expected = "";

        REQUIRE(string_vec.size() == 2);
        REQUIRE(string_vec.capacity() == 3);

        REQUIRE_THAT(string_vec, AllMatch(Equals(expected)));
    }

    SECTION("constructor which fills vec with n copies of a value works")
    {
        constexpr auto vec_val = 3.0f;

        zth::InPlaceVector<float, 7> float_vec(4, vec_val);

        REQUIRE(float_vec.size() == 4);
        REQUIRE(float_vec.capacity() == 7);

        REQUIRE_THAT(float_vec, AllMatch(Equals(vec_val)));
    }

    SECTION("initializer list constructor works")
    {
        zth::InPlaceVector<char, 5> char_vec = { 'A', 'p', 'p', 'l', 'e' };

        REQUIRE(char_vec.size() == 5);
        REQUIRE(char_vec.capacity() == 5);

        size_t i = 0;
        REQUIRE(char_vec[i++] == 'A');
        REQUIRE(char_vec[i++] == 'p');
        REQUIRE(char_vec[i++] == 'p');
        REQUIRE(char_vec[i++] == 'l');
        REQUIRE(char_vec[i++] == 'e');
    }
}

TEST_CASE("InPlaceVector copy and move operations work", "[InPlaceVector]")
{
    // TODO
}
