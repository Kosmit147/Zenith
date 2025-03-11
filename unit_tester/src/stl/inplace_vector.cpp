#include <array>
#include <utility>

#include <zenith/stl/inplace_vector.hpp>

#include "lifetime_helper.hpp"
#include "matchers.hpp"

using Catch::Matchers::AllMatch;
using Catch::Matchers::RangeEquals;

TEST_CASE("InPlaceVector can be constructed", "[InPlaceVector]")
{
    SECTION("default constructor works")
    {
        zth::InPlaceVector<int, 10> int_vec{};

        REQUIRE(int_vec.empty());
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

        std::size_t i = 0;
        REQUIRE(char_vec[i++] == 'A');
        REQUIRE(char_vec[i++] == 'p');
        REQUIRE(char_vec[i++] == 'p');
        REQUIRE(char_vec[i++] == 'l');
        REQUIRE(char_vec[i++] == 'e');
    }
}

TEST_CASE("InPlaceVector can be copied and moved", "[InPlaceVector]")
{
    zth::InPlaceVector<int, 3> source = { 1, 2, 3 };

    SECTION("copy constructor works")
    {
        auto dest{ source };

        REQUIRE_THAT(dest, RangeEquals(source));

        REQUIRE(dest.size() == source.size());
        REQUIRE(dest.capacity() == source.capacity());
    }

    SECTION("copy assignment works")
    {
        decltype(source) dest;

        dest = source;

        REQUIRE_THAT(dest, RangeEquals(source));

        REQUIRE(dest.size() == source.size());
        REQUIRE(dest.capacity() == source.capacity());
    }

    SECTION("move constructor works")
    {
        auto tmp{ source };
        auto dest{ std::move(tmp) };

        REQUIRE_THAT(dest, RangeEquals(source));

        REQUIRE(dest.size() == source.size());
        REQUIRE(dest.capacity() == source.capacity());
    }

    SECTION("move assignment works")
    {
        auto tmp{ source };
        decltype(source) dest;

        dest = std::move(tmp);

        REQUIRE_THAT(dest, RangeEquals(source));

        REQUIRE(dest.size() == source.size());
        REQUIRE(dest.capacity() == source.capacity());
    }
}

TEST_CASE("can access elements inside an InPlaceVector", "[InPlaceVector]")
{
    const zth::InPlaceVector<int, 5> vec = { 1, 2, 3, 4, 5 };

    REQUIRE_THROWS(vec.at(static_cast<zth::usize>(-1)));
    REQUIRE_THROWS(vec.at(5));
    REQUIRE_THROWS(vec.at(100));

    REQUIRE(vec.at(0) == 1);
    REQUIRE(vec.at(2) == 3);
    REQUIRE(vec.at(4) == 5);

    REQUIRE(vec[0] == 1);
    REQUIRE(vec[2] == 3);
    REQUIRE(vec[4] == 5);

    REQUIRE(vec.front() == 1);
    REQUIRE(vec.back() == 5);

    const auto* data_ptr = vec.data();

    std::size_t i = 0;
    REQUIRE(data_ptr[i++] == 1);
    REQUIRE(data_ptr[i++] == 2);
    REQUIRE(data_ptr[i++] == 3);
    REQUIRE(data_ptr[i++] == 4);
    REQUIRE(data_ptr[i++] == 5);
}

TEST_CASE("can iterate over elements of an InPlaceVector", "[InPlaceVector]")
{
    zth::InPlaceVector<double, 4> elems = { 0.0, 1.0, 2.0, 3.0 };
    std::array expected = { 0.0, 1.0, 2.0, 3.0 };

    std::size_t i = 0;

    for (const auto& elem : elems)
    {
        REQUIRE(elem == expected[i]);
        i++;
    }
}

TEST_CASE("InPlaceVector can be modified", "[InPlaceVector]")
{
    zth::InPlaceVector<int, 100> vec = { 1, 2, 3 };

    REQUIRE_FALSE(vec.empty());
    REQUIRE(vec.size() == 3);
    REQUIRE(vec.capacity() == 100);

    SECTION("can use at() or operator[] to modify")
    {
        vec.at(0) = 100;
        REQUIRE(vec.at(0) == 100);

        vec[1] = 200;
        REQUIRE(vec[1] == 200);
    }

    SECTION("can push back")
    {
        std::array expected = { 1, 2, 3, 4, 5, 6 };

        vec.push_back(4);
        vec.push_back(5);
        vec.push_back(std::move(6));

        REQUIRE(vec.size() == expected.size());
        REQUIRE_THAT(vec, RangeEquals(expected));
    }

    SECTION("can emplace back")
    {
        std::array expected = { 1, 2, 3, 4, 5, 6 };

        vec.emplace_back(4);
        vec.emplace_back(5);
        vec.emplace_back(std::move(6));

        REQUIRE(vec.size() == expected.size());
        REQUIRE_THAT(vec, RangeEquals(expected));
    }

    SECTION("can pop back")
    {
        vec.pop_back();
        vec.pop_back();

        REQUIRE(vec.size() == 1);
        REQUIRE(vec[0] == 1);
    }

    SECTION("can clear")
    {
        vec.clear();

        REQUIRE(vec.size() == 0);
        REQUIRE(vec.capacity() == 100);
    }

    SECTION("can mix modifying operations")
    {
        vec.clear();

        REQUIRE(vec.size() == 0);
        REQUIRE(vec.capacity() == 100);

        vec.push_back(3);
        vec.push_back(2);
        vec.push_back(1);

        std::array expected_1 = { 3, 2, 1 };

        REQUIRE(vec.size() == expected_1.size());
        REQUIRE_THAT(vec, RangeEquals(expected_1));

        vec.pop_back();
        vec.pop_back();
        vec.emplace_back(60);

        std::array expected_2 = { 3, 60 };

        REQUIRE(vec.size() == expected_2.size());
        REQUIRE_THAT(vec, RangeEquals(expected_2));

        REQUIRE(vec.capacity() == 100);
    }
}

TEST_CASE("objects get properly constructed and destructed inside an InPlaceVector", "[InPlaceVector]")
{
    LifetimeHelper::reset();

    {
        zth::InPlaceVector<LifetimeHelper, 10> vec(2);

        vec.push_back(LifetimeHelper{});
        vec.push_back(LifetimeHelper{});
        vec.pop_back();

        LifetimeHelper obj;

        vec.emplace_back();
        vec.push_back(obj);
        vec.push_back(std::move(obj));
    }

    REQUIRE(LifetimeHelper::ctors == LifetimeHelper::dtors);

    REQUIRE(LifetimeHelper::ctors == 10);
    REQUIRE(LifetimeHelper::default_ctors == 6);
    REQUIRE(LifetimeHelper::copy_ctors == 1);
    REQUIRE(LifetimeHelper::move_ctors == 3);

    REQUIRE(LifetimeHelper::dtors == 10);
}
