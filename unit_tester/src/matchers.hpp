#pragma once

#include <string>

#include <zenith/log/format.hpp>

template<typename T> struct EqualsMatcher : Catch::Matchers::MatcherGenericBase
{
    EqualsMatcher(const T& val) : _val(val) {}

    auto match(const T& other) const -> bool { return _val == other; }
    auto describe() const -> std::string override { return zth::format("Equals: {}", _val); }

private:
    const T& _val;
};

template<typename T> auto Equals(const T& val) -> EqualsMatcher<T>
{
    return EqualsMatcher<T>{ val };
}
