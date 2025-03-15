#pragma once

#include <concepts>
#include <iterator>
#include <span>
#include <type_traits>

#include "zenith/core/assert.hpp"
#include "zenith/core/typedefs.hpp"
#include "zenith/util/optional.hpp"
#include "zenith/util/reference.hpp"

// @todo: Implement iterators (use iterator concepts from standard library).
// @todo: Implement reverse iterators (rbegin, rend).

namespace zth {

// An interface to be implemented by a range which represents data contiguously stored in memory.
//
// # Requires:
// - data()
// - cbegin()
// - cend()
// - size()
// - capacity()
// - value_type
//
// # Provides:
// - begin()
// - end()
// - empty()
// - ssize()
// - size_bytes()
// - span()
// - front()
// - back()
// - operator[]
// - at()
class ContiguousRangeInterface
{
public:
    [[nodiscard]] constexpr auto begin(this auto&& self) -> decltype(auto);
    [[nodiscard]] constexpr auto end(this auto&& self) -> decltype(auto);

    [[nodiscard]] constexpr auto empty(this auto&& self) -> bool;
    [[nodiscard]] constexpr auto ssize(this auto&& self) -> auto;
    [[nodiscard]] constexpr auto size_bytes(this auto&& self) -> usize;
    [[nodiscard]] constexpr auto span(this auto&& self) -> auto;

    [[nodiscard]] constexpr auto front(this auto&& self) -> decltype(auto);
    [[nodiscard]] constexpr auto back(this auto&& self) -> decltype(auto);

    [[nodiscard]] constexpr auto operator[](this auto&& self, std::integral auto index) -> decltype(auto);
    [[nodiscard]] constexpr auto at(this auto&& self, std::integral auto index) -> auto;

protected:
    explicit ContiguousRangeInterface() = default;
};

constexpr auto ContiguousRangeInterface::begin(this auto&& self) -> decltype(auto)
{
    return self.cbegin();
}

constexpr auto ContiguousRangeInterface::end(this auto&& self) -> decltype(auto)
{
    return self.cend();
}

constexpr auto ContiguousRangeInterface::empty(this auto&& self) -> bool
{
    return self.size() == 0;
}

constexpr auto ContiguousRangeInterface::ssize(this auto&& self) -> auto
{
    using Self = std::remove_cvref_t<decltype(self)>;
    using size_type = std::invoke_result_t<decltype(&Self::size), decltype(self)>;
    using return_type = std::make_signed_t<size_type>;

    return static_cast<return_type>(self.size());
}

constexpr auto ContiguousRangeInterface::size_bytes(this auto&& self) -> usize
{
    using Self = std::remove_cvref_t<decltype(self)>;
    using value_type = typename Self::value_type;

    return self.size() * sizeof(value_type);
}

constexpr auto ContiguousRangeInterface::span(this auto&& self) -> auto
{
    return std::span{ self.begin(), self.end() };
}

constexpr auto ContiguousRangeInterface::front(this auto&& self) -> decltype(auto)
{
    ZTH_ASSERT(!self.empty());
    return *self.begin();
}

constexpr auto ContiguousRangeInterface::back(this auto&& self) -> decltype(auto)
{
    ZTH_ASSERT(!self.empty());
    return *std::prev(self.end());
}

constexpr auto ContiguousRangeInterface::operator[](this auto&& self, std::integral auto index) -> decltype(auto)
{
    ZTH_ASSERT(index < self.size());
    return *std::next(self.begin(), index);
}

constexpr auto ContiguousRangeInterface::at(this auto&& self, std::integral auto index) -> auto
{
    using Self = std::remove_cvref_t<decltype(self)>;
    using value_type = typename Self::value_type;

    using return_type = std::conditional_t<std::is_const_v<std::remove_reference_t<decltype(self)>>,
                                           Reference<const value_type>, Reference<value_type>>;

    if (index >= self.size())
        return Optional<return_type>{ nil };

    return zth::make_optional<return_type>(self[index]);
}

} // namespace zth
