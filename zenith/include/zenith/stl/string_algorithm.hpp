#pragma once

#include <cctype>

#include "zenith/core/typedefs.hpp"
#include "zenith/log/format.hpp"
#include "zenith/stl/string.hpp"
#include "zenith/util/optional.hpp"

namespace zth {

struct FindSubstrResult
{
    usize found_at;
    StringView result;

    [[nodiscard]] auto operator==(const FindSubstrResult&) const -> bool = default;
};

[[nodiscard]] auto substr_between(StringView text, char delim) -> Optional<StringView>;
[[nodiscard]] auto substr_between(StringView text, char opening_delim, char closing_delim) -> Optional<StringView>;

[[nodiscard]] auto substr_between_at_offset(StringView text, usize offset, char delim) -> Optional<StringView>;
[[nodiscard]] auto substr_between_at_offset(StringView text, usize offset, char opening_delim, char closing_delim)
    -> Optional<StringView>;

[[nodiscard]] auto find_substr_between(StringView text, char delim) -> Optional<FindSubstrResult>;
[[nodiscard]] auto find_substr_between(StringView text, char opening_delim, char closing_delim)
    -> Optional<FindSubstrResult>;

[[nodiscard]] auto find_substr_between_at_offset(StringView text, usize offset, char delim)
    -> Optional<FindSubstrResult>;
[[nodiscard]] auto find_substr_between_at_offset(StringView text, usize offset, char opening_delim, char closing_delim)
    -> Optional<FindSubstrResult>;

[[nodiscard]] inline auto case_insensitive_equal(char a, char b) -> bool
{
    return std::tolower(a) == std::tolower(b);
}

[[nodiscard]] auto case_insensitive_find(StringView text, StringView search) -> Optional<usize>;
[[nodiscard]] auto case_insensitive_contains(StringView text, StringView search) -> bool;

} // namespace zth

ZTH_DECLARE_FORMATTER(zth::FindSubstrResult);
