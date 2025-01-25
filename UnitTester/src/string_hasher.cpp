#include <catch2/catch_test_macros.hpp>

#include <string>
#include <string_view>

#include <zenith/utility/string_hasher.hpp>

TEST_CASE("StringHasher hashes strings transparently", "[StringHasher]")
{
    zth::StringHasher hasher;

    const auto str = "Hello World";

    auto hash_from_c_str = hasher(str);
    auto hash_from_string_view = hasher(std::string_view{ str });
    auto hash_from_string = hasher(std::string{ str });

    REQUIRE(hash_from_c_str == hash_from_string_view);
    REQUIRE(hash_from_string_view == hash_from_string);
}
