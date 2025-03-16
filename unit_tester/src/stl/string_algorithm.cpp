#include <zenith/stl/string.hpp>

#include "matchers.hpp"

using namespace zth::string_view_literals;

TEST_CASE("substr_between", "[string]")
{
    auto result = zth::substr_between("<example>", '<', '>');

    REQUIRE(result.has_value());
    REQUIRE_THAT(*result, Equals("example"_sv));

    result = zth::substr_between("\'example\'", '\'');

    REQUIRE(result.has_value());
    REQUIRE_THAT(*result, Equals("example"_sv));

    result = zth::substr_between("<example", '<', '>');
    REQUIRE_FALSE(result.has_value());

    result = zth::substr_between("example>", '<', '>');
    REQUIRE_FALSE(result.has_value());
}

TEST_CASE("substr_between_with_offset", "[string]")
{
    auto result = zth::substr_between_at_offset("<1>  <2>", 1, '<', '>');

    REQUIRE(result.has_value());
    REQUIRE_THAT(*result, Equals("2"_sv));

    result = zth::substr_between_at_offset("<1>  \'2\'", 5, '\'');

    REQUIRE(result.has_value());
    REQUIRE_THAT(*result, Equals("2"_sv));

    result = zth::substr_between_at_offset("><12>><example>", 2, '<', '>');

    REQUIRE(result.has_value());
    REQUIRE_THAT(*result, Equals("example"_sv));

    result = zth::substr_between_at_offset("<example", 1, '<', '>');
    REQUIRE_FALSE(result.has_value());

    result = zth::substr_between_at_offset("\'example", 0, '\'');
    REQUIRE_FALSE(result.has_value());

    result = zth::substr_between_at_offset("example>", 4, '<', '>');
    REQUIRE_FALSE(result.has_value());

    result = zth::substr_between_at_offset("example\'", 4, '\'');
    REQUIRE_FALSE(result.has_value());

    result = zth::substr_between_at_offset("", 1000, ' ');
    REQUIRE_FALSE(result.has_value());
}

TEST_CASE("find_substr_between", "[string]")
{
    auto result = zth::find_substr_between("  <example>", '<', '>');

    REQUIRE(result.has_value());
    REQUIRE_THAT(*result, Equals(zth::FindSubstrResult{ .found_at = 3, .result = "example" }));

    result = zth::find_substr_between("  \'example\'", '\'');

    REQUIRE(result.has_value());
    REQUIRE_THAT(*result, Equals(zth::FindSubstrResult{ .found_at = 3, .result = "example" }));

    result = zth::find_substr_between("<example", '<', '>');
    REQUIRE_FALSE(result.has_value());

    result = zth::find_substr_between("example>", '<', '>');
    REQUIRE_FALSE(result.has_value());
}

TEST_CASE("find_substr_between_with_offset", "[string]")
{
    auto result = zth::find_substr_between_at_offset("<1>  <2>", 1, '<', '>');

    REQUIRE(result.has_value());
    REQUIRE_THAT(*result, Equals(zth::FindSubstrResult{ .found_at = 6, .result = "2" }));

    result = zth::find_substr_between_at_offset("<1>  \'2\'", 5, '\'');

    REQUIRE(result.has_value());
    REQUIRE_THAT(*result, Equals(zth::FindSubstrResult{ .found_at = 6, .result = "2" }));

    result = zth::find_substr_between_at_offset("<example", 1, '<', '>');
    REQUIRE_FALSE(result.has_value());

    result = zth::find_substr_between_at_offset("\'example", 0, '\'');
    REQUIRE_FALSE(result.has_value());

    result = zth::find_substr_between_at_offset("example>", 4, '<', '>');
    REQUIRE_FALSE(result.has_value());

    result = zth::find_substr_between_at_offset("example\'", 4, '\'');
    REQUIRE_FALSE(result.has_value());

    result = zth::find_substr_between_at_offset("", 1000, ' ');
    REQUIRE_FALSE(result.has_value());
}
