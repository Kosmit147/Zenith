#pragma once

#include <entt/core/hashed_string.hpp>

#include "zenith/core/typedefs.hpp"

namespace zth {

using HashedString = entt::hashed_string;

namespace hashed_string_literals {

[[nodiscard]] constexpr auto operator""_hs(const char* text, usize length) -> HashedString
{
    return entt::hashed_string{ text, length };
}

} // namespace hashed_string_literals

} // namespace zth
