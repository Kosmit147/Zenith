#include <catch2/catch_test_macros.hpp>

#include <Zenith/Utility/Cleanup.hpp>

TEST_CASE("cleanup works", "[Cleanup]")
{
    auto cleaned_up = false;

    {
        zth::Cleanup cleanup{ [&] { cleaned_up = true; } };
    }

    REQUIRE(cleaned_up);
}

TEST_CASE("can dismiss cleanup", "[Cleanup]")
{
    auto cleaned_up = false;

    {
        zth::Cleanup cleanup{ [&] { cleaned_up = true; } };
        cleanup.dismiss();
    }

    REQUIRE_FALSE(cleaned_up);
}

TEST_CASE("can release cleanup", "[Cleanup]")
{
    auto cleaned_up = false;

    {
        zth::Cleanup cleanup{ [&] { cleaned_up = true; } };
        cleanup.release();
        REQUIRE(cleaned_up);
    }

    REQUIRE(cleaned_up);
}
