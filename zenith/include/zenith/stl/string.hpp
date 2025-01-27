#pragma once

#include <spdlog/fmt/fmt.h>

#include <optional>
#include <string_view>

#include "zenith/core/typedefs.hpp"

namespace zth::string {

struct FindResult
{
    usize found_at;
    std::string_view result;

    [[nodiscard]] auto operator==(const FindResult&) const -> bool = default;
};

[[nodiscard]] auto substr_between(std::string_view text, char delim) -> std::optional<std::string_view>;
[[nodiscard]] auto substr_between(std::string_view text, char opening_delim,
                                  char closing_delim) -> std::optional<std::string_view>;

[[nodiscard]] auto substr_between_with_offset(std::string_view text, usize offset,
                                              char delim) -> std::optional<std::string_view>;
[[nodiscard]] auto substr_between_with_offset(std::string_view text, usize offset, char opening_delim,
                                              char closing_delim) -> std::optional<std::string_view>;

[[nodiscard]] auto find_substr_between(std::string_view text, char delim) -> std::optional<FindResult>;
[[nodiscard]] auto find_substr_between(std::string_view text, char opening_delim,
                                       char closing_delim) -> std::optional<FindResult>;

[[nodiscard]] auto find_substr_between_with_offset(std::string_view text, usize offset,
                                                   char delim) -> std::optional<FindResult>;
[[nodiscard]] auto find_substr_between_with_offset(std::string_view text, usize offset, char opening_delim,
                                                   char closing_delim) -> std::optional<FindResult>;

} // namespace zth::string

template<> struct fmt::formatter<zth::string::FindResult> : formatter<std::string>
{
    static auto format(const zth::string::FindResult& find_result, format_context& ctx) -> decltype(ctx.out())
    {
        auto& [found_at, result] = find_result;
        return format_to(ctx.out(), "{{ .found_at = {}, .result = {} }}", found_at, result);
    }
};
