#include <memory>
#include <type_traits>

#include <zenith/memory/managed.hpp>

#include "lifetime_helper.hpp"

TEST_CASE("Cannot copy UniquePtr", "[UniquePtr]")
{
    static_assert(!std::is_copy_constructible_v<zth::UniquePtr<int>>);
    static_assert(!std::is_copy_assignable_v<zth::UniquePtr<int>>);
}

TEST_CASE("Can construct and move UniquePtr", "[UniquePtr]")
{
    LifetimeHelper::reset();

    {
        zth::UniquePtr<LifetimeHelper> ptr1;
        ptr1 = zth::make_unique<LifetimeHelper>();

        zth::UniquePtr<LifetimeHelper> ptr2;
        ptr2 = std::move(ptr1);
    }

    REQUIRE(LifetimeHelper::ctors == LifetimeHelper::dtors);

    REQUIRE(LifetimeHelper::ctors == 1);
    REQUIRE(LifetimeHelper::dtors == 1);
}

TEST_CASE("Can allocate an object for overwrite via UniquePtr", "[UniquePtr]")
{
    LifetimeHelper::reset();

    {
        zth::UniquePtr<LifetimeHelper> ptr1;
        ptr1 = zth::make_unique_for_overwrite<LifetimeHelper>();

        zth::UniquePtr<LifetimeHelper> ptr2;
        ptr2 = std::move(ptr1);
        std::construct_at(ptr2.get());
    }

    REQUIRE(LifetimeHelper::ctors == LifetimeHelper::dtors);

    REQUIRE(LifetimeHelper::ctors == 1);
    REQUIRE(LifetimeHelper::dtors == 1);
}

TEST_CASE("Can create an array of objects via UniquePtr", "[UniquePtr]")
{
    LifetimeHelper::reset();

    {
        zth::UniquePtr<LifetimeHelper[]> ptr1;
        ptr1 = zth::make_unique<LifetimeHelper[]>(10);
        REQUIRE(ptr1.count() == 10);

        zth::UniquePtr<LifetimeHelper[]> ptr2;
        ptr2 = std::move(ptr1);
        REQUIRE(ptr2.count() == 10);
    }

    REQUIRE(LifetimeHelper::ctors == LifetimeHelper::dtors);

    REQUIRE(LifetimeHelper::ctors == 10);
    REQUIRE(LifetimeHelper::dtors == 10);
}

TEST_CASE("Can allocate an array of objects for overwrite via UniquePtr", "[UniquePtr]")
{
    LifetimeHelper::reset();

    {
        zth::UniquePtr<LifetimeHelper[]> ptr1;
        ptr1 = zth::make_unique_for_overwrite<LifetimeHelper[]>(10);
        REQUIRE(ptr1.count() == 10);

        zth::UniquePtr<LifetimeHelper[]> ptr2;
        ptr2 = std::move(ptr1);
        REQUIRE(ptr2.count() == 10);
    }

    REQUIRE(LifetimeHelper::ctors != LifetimeHelper::dtors);

    REQUIRE(LifetimeHelper::ctors == 0); // We didn't construct any objects, we just allocated the memory.
    REQUIRE(LifetimeHelper::dtors == 10);
}

TEST_CASE("UniquePtr is polymorphic", "[UniquePtr]")
{
    auto base_flag = false;
    auto derived_flag = false;

    struct Base
    {
        explicit Base(bool& base_flag) : base_flag{ base_flag } {}
        virtual ~Base() { base_flag = true; }
        bool& base_flag;
    };

    struct Derived : Base
    {
        explicit Derived(bool& base_flag, bool& derived_flag) : Base(base_flag), derived_flag{ derived_flag } {}
        ~Derived() override { derived_flag = true; }
        bool& derived_flag;
    };

    {
        zth::UniquePtr<Derived> derived_ptr = zth::make_unique<Derived>(base_flag, derived_flag);
        zth::UniquePtr<Base> base_ptr = std::move(derived_ptr);
    }

    REQUIRE(base_flag);
    REQUIRE(derived_flag);
}

TEST_CASE("Can dereference UniquePtr", "[UniquePtr]")
{
    zth::UniquePtr<int> num = zth::make_unique<int>(5);
    REQUIRE(*num == 5);

    zth::UniquePtr<int[]> arr = zth::make_unique<int[]>(3);

    REQUIRE(arr.count() == 3);
    REQUIRE(arr[0] == 0);
    REQUIRE(arr[1] == 0);
    REQUIRE(arr[2] == 0);

    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 3;

    REQUIRE(arr[0] == 1);
    REQUIRE(arr[1] == 2);
    REQUIRE(arr[2] == 3);
}
