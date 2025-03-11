#include <array>
#include <cstddef>

#include <zenith/memory/memory.hpp>

TEST_CASE("pointer alignment functions", "[Memory]")
{
    const std::array int_pointers = {
        reinterpret_cast<int*>(0b0000), reinterpret_cast<int*>(0b0001), reinterpret_cast<int*>(0b0010),
        reinterpret_cast<int*>(0b0011), reinterpret_cast<int*>(0b0100),
    };

    const std::array expected_aligned_int_pointers = {
        reinterpret_cast<int*>(0b0000), reinterpret_cast<int*>(0b0100), reinterpret_cast<int*>(0b0100),
        reinterpret_cast<int*>(0b0100), reinterpret_cast<int*>(0b0100),
    };

    const std::array expected_aligned_down_int_pointers = {
        reinterpret_cast<int*>(0b0000), reinterpret_cast<int*>(0b0000), reinterpret_cast<int*>(0b0000),
        reinterpret_cast<int*>(0b0000), reinterpret_cast<int*>(0b0100),
    };

    const std::array byte_pointers = {
        reinterpret_cast<std::byte*>(0b0000),
        reinterpret_cast<std::byte*>(0b0001),
        reinterpret_cast<std::byte*>(0b0010),
    };

    const std::array expected_aligned_byte_pointers = {
        reinterpret_cast<std::byte*>(0b0000),
        reinterpret_cast<std::byte*>(0b0001),
        reinterpret_cast<std::byte*>(0b0010),
    };

    const std::array expected_aligned_down_byte_pointers = {
        reinterpret_cast<std::byte*>(0b0000),
        reinterpret_cast<std::byte*>(0b0001),
        reinterpret_cast<std::byte*>(0b0010),
    };

    SECTION("align")
    {
        for (std::size_t i = 0; i < int_pointers.size(); i++)
        {
            auto ptr = int_pointers[i];
            REQUIRE(zth::memory::aligned(ptr) == expected_aligned_int_pointers[i]);
            zth::memory::align(ptr);
            REQUIRE(ptr == expected_aligned_int_pointers[i]);
        }

        for (std::size_t i = 0; i < byte_pointers.size(); i++)
        {
            auto ptr = byte_pointers[i];
            REQUIRE(zth::memory::aligned(ptr) == expected_aligned_byte_pointers[i]);
            zth::memory::align(ptr);
            REQUIRE(ptr == expected_aligned_byte_pointers[i]);
        }
    }

    SECTION("align_down")
    {
        for (std::size_t i = 0; i < int_pointers.size(); i++)
        {
            auto ptr = int_pointers[i];
            REQUIRE(zth::memory::aligned_down(ptr) == expected_aligned_down_int_pointers[i]);
            zth::memory::align_down(ptr);
            REQUIRE(ptr == expected_aligned_down_int_pointers[i]);
        }

        for (std::size_t i = 0; i < byte_pointers.size(); i++)
        {
            auto ptr = byte_pointers[i];
            REQUIRE(zth::memory::aligned_down(ptr) == expected_aligned_down_byte_pointers[i]);
            zth::memory::align_down(ptr);
            REQUIRE(ptr == expected_aligned_down_byte_pointers[i]);
        }
    }
}
