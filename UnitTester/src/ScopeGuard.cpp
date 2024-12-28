#include <catch2/catch_test_macros.hpp>

#include <Zenith/Utility/ScopeGuard.hpp>

TEST_CASE("ScopeGuard works", "[ScopeGuard]")
{
    auto cleaned_up = false;

    {
        zth::ScopeGuard guard{ [&] { cleaned_up = true; } };
    }

    REQUIRE(cleaned_up);
}

TEST_CASE("can dismiss ScopeGuard", "[ScopeGuard]")
{
    auto cleaned_up = false;

    {
        zth::ScopeGuard guard{ [&] { cleaned_up = true; } };
        guard.dismiss();
    }

    REQUIRE_FALSE(cleaned_up);
}

TEST_CASE("can release ScopeGuard", "[ScopeGuard]")
{
    auto cleaned_up = false;

    {
        zth::ScopeGuard guard{ [&] { cleaned_up = true; } };
        guard.release();
        REQUIRE(cleaned_up);
    }

    REQUIRE(cleaned_up);
}
