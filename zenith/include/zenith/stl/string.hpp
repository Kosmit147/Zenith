#pragma once

#include <memory>
#include <string>
#include <string_view>

#include "zenith/core/typedefs.hpp"

namespace zth {

template<typename Char = char, typename CharTraits = std::char_traits<Char>, typename Allocator = std::allocator<Char>>
using GenericString = std::basic_string<Char, CharTraits, Allocator>;
template<typename Char = char> using GenericStringView = std::basic_string_view<Char>;

using String = GenericString<char, std::char_traits<char>, std::allocator<char>>;
using StringView = GenericStringView<char>;

namespace string_view_literals {

[[nodiscard]] constexpr auto operator""_sv(const char* text, usize length) -> StringView
{
    return StringView{ text, length };
}

} // namespace string_view_literals

} // namespace zth
