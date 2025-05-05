#include <zenith/stl/hash.hpp>
#include <zenith/stl/string.hpp>

TEST_CASE("StringHash hashes strings transparently", "[Hash]")
{
    zth::StringHash hasher;

    const auto str = "Hello World";

    auto hash_from_c_str = hasher(str);
    auto hash_from_string_view = hasher(zth::StringView{ str });
    auto hash_from_string = hasher(zth::String{ str });

    REQUIRE(hash_from_c_str == hash_from_string_view);
    REQUIRE(hash_from_string_view == hash_from_string);
}
