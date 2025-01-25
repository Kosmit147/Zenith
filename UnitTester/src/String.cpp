#include <catch2/catch_test_macros.hpp>

#include <string_view>

#include <Zenith/Utility/String.hpp>

#include "Matchers.hpp"

using namespace std::string_view_literals;

TEST_CASE("substr_between", "[string]")
{
    auto result = zth::string::substr_between("<example>", '<', '>');

    REQUIRE(result.has_value());
    REQUIRE_THAT(*result, Equals("example"sv));

    result = zth::string::substr_between("\'example\'", '\'');

    REQUIRE(result.has_value());
    REQUIRE_THAT(*result, Equals("example"sv));

    result = zth::string::substr_between("<example", '<', '>');
    REQUIRE_FALSE(result.has_value());

    result = zth::string::substr_between("example>", '<', '>');
    REQUIRE_FALSE(result.has_value());
}

TEST_CASE("substr_between_with_offset", "[string]")
{
    auto result = zth::string::substr_between_with_offset("<1>  <2>", 1, '<', '>');

    REQUIRE(result.has_value());
    REQUIRE_THAT(*result, Equals("2"sv));

    result = zth::string::substr_between_with_offset("<1>  \'2\'", 5, '\'');

    REQUIRE(result.has_value());
    REQUIRE_THAT(*result, Equals("2"sv));

    result = zth::string::substr_between_with_offset("><12>><example>", 2, '<', '>');

    REQUIRE(result.has_value());
    REQUIRE_THAT(*result, Equals("example"sv));

    result = zth::string::substr_between_with_offset("<example", 1, '<', '>');
    REQUIRE_FALSE(result.has_value());

    result = zth::string::substr_between_with_offset("\'example", 0, '\'');
    REQUIRE_FALSE(result.has_value());

    result = zth::string::substr_between_with_offset("example>", 4, '<', '>');
    REQUIRE_FALSE(result.has_value());

    result = zth::string::substr_between_with_offset("example\'", 4, '\'');
    REQUIRE_FALSE(result.has_value());

    result = zth::string::substr_between_with_offset("", 1000, ' ');
    REQUIRE_FALSE(result.has_value());
}

TEST_CASE("find_substr_between", "[string]")
{
    auto result = zth::string::find_substr_between("  <example>", '<', '>');

    REQUIRE(result.has_value());
    REQUIRE_THAT(*result, Equals(zth::string::FindResult{ .found_at = 3, .result = "example" }));

    result = zth::string::find_substr_between("  \'example\'", '\'');

    REQUIRE(result.has_value());
    REQUIRE_THAT(*result, Equals(zth::string::FindResult{ .found_at = 3, .result = "example" }));

    result = zth::string::find_substr_between("<example", '<', '>');
    REQUIRE_FALSE(result.has_value());

    result = zth::string::find_substr_between("example>", '<', '>');
    REQUIRE_FALSE(result.has_value());
}

TEST_CASE("find_substr_between_with_offset", "[string]")
{
    auto result = zth::string::find_substr_between_with_offset("<1>  <2>", 1, '<', '>');

    REQUIRE(result.has_value());
    REQUIRE_THAT(*result, Equals(zth::string::FindResult{ .found_at = 6, .result = "2" }));

    result = zth::string::find_substr_between_with_offset("<1>  \'2\'", 5, '\'');

    REQUIRE(result.has_value());
    REQUIRE_THAT(*result, Equals(zth::string::FindResult{ .found_at = 6, .result = "2" }));

    result = zth::string::find_substr_between_with_offset("<example", 1, '<', '>');
    REQUIRE_FALSE(result.has_value());

    result = zth::string::find_substr_between_with_offset("\'example", 0, '\'');
    REQUIRE_FALSE(result.has_value());

    result = zth::string::find_substr_between_with_offset("example>", 4, '<', '>');
    REQUIRE_FALSE(result.has_value());

    result = zth::string::find_substr_between_with_offset("example\'", 4, '\'');
    REQUIRE_FALSE(result.has_value());

    result = zth::string::find_substr_between_with_offset("", 1000, ' ');
    REQUIRE_FALSE(result.has_value());
}
