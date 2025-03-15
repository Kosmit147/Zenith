#pragma once

#include <string_view>

#include "zenith/core/typedefs.hpp"
#include "zenith/log/format.hpp"
#include "zenith/util/optional.hpp"

namespace zth::string {

struct FindResult
{
    usize found_at;
    std::string_view result;

    [[nodiscard]] auto operator==(const FindResult&) const -> bool = default;
};

[[nodiscard]] auto substr_between(std::string_view text, char delim) -> Optional<std::string_view>;
[[nodiscard]] auto substr_between(std::string_view text, char opening_delim, char closing_delim)
    -> Optional<std::string_view>;

[[nodiscard]] auto substr_between_with_offset(std::string_view text, usize offset, char delim)
    -> Optional<std::string_view>;
[[nodiscard]] auto substr_between_with_offset(std::string_view text, usize offset, char opening_delim,
                                              char closing_delim) -> Optional<std::string_view>;

[[nodiscard]] auto find_substr_between(std::string_view text, char delim) -> Optional<FindResult>;
[[nodiscard]] auto find_substr_between(std::string_view text, char opening_delim, char closing_delim)
    -> Optional<FindResult>;

[[nodiscard]] auto find_substr_between_with_offset(std::string_view text, usize offset, char delim)
    -> Optional<FindResult>;
[[nodiscard]] auto find_substr_between_with_offset(std::string_view text, usize offset, char opening_delim,
                                                   char closing_delim) -> Optional<FindResult>;

} // namespace zth::string

ZTH_DECLARE_FORMATTER(zth::string::FindResult);
