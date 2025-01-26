#include <catch2/catch_test_macros.hpp>

#include <zenith/utility/defer.hpp>

TEST_CASE("defer works", "[Defer]")
{
    auto cleaned_up = false;

    {
        zth::Defer cleanup{ [&] { cleaned_up = true; } };
    }

    REQUIRE(cleaned_up);
}

TEST_CASE("can dismiss defer", "[Defer]")
{
    auto cleaned_up = false;

    {
        zth::Defer cleanup{ [&] { cleaned_up = true; } };
        cleanup.dismiss();
    }

    REQUIRE_FALSE(cleaned_up);
}

TEST_CASE("can release defer", "[Defer]")
{
    auto cleaned_up = false;

    {
        zth::Defer cleanup{ [&] { cleaned_up = true; } };
        cleanup.release();
        REQUIRE(cleaned_up);
    }

    REQUIRE(cleaned_up);
}
