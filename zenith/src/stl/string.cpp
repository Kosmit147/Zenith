#include "zenith/stl/string.hpp"

namespace zth::string {

auto substr_between(std::string_view text, char delim) -> std::optional<std::string_view>
{
    return substr_between(text, delim, delim);
}

auto substr_between(std::string_view text, char opening_delim, char closing_delim) -> std::optional<std::string_view>
{
    return substr_between_with_offset(text, 0, opening_delim, closing_delim);
}

auto substr_between_with_offset(std::string_view text, usize offset, char delim) -> std::optional<std::string_view>
{
    return substr_between_with_offset(text, offset, delim, delim);
}

auto substr_between_with_offset(std::string_view text, usize offset, char opening_delim,
                                char closing_delim) -> std::optional<std::string_view>
{
    auto start = text.find(opening_delim, offset);

    if (start == std::string_view::npos)
        return {};

    auto end = text.find(closing_delim, start + 1);

    if (end == std::string_view::npos)
        return {};

    auto found_at = start + 1;
    auto length = end - start - 1;
    return std::make_optional(text.substr(found_at, length));
}

auto find_substr_between(std::string_view text, char delim) -> std::optional<FindResult>
{
    return find_substr_between(text, delim, delim);
}

auto find_substr_between(std::string_view text, char opening_delim, char closing_delim) -> std::optional<FindResult>
{
    return find_substr_between_with_offset(text, 0, opening_delim, closing_delim);
}

auto find_substr_between_with_offset(std::string_view text, usize offset, char delim) -> std::optional<FindResult>
{
    return find_substr_between_with_offset(text, offset, delim, delim);
}

auto find_substr_between_with_offset(std::string_view text, usize offset, char opening_delim,
                                     char closing_delim) -> std::optional<FindResult>
{
    auto start = text.find(opening_delim, offset);

    if (start == std::string_view::npos)
        return {};

    auto end = text.find(closing_delim, start + 1);

    if (end == std::string_view::npos)
        return {};

    auto found_at = start + 1;
    auto length = end - start - 1;
    return std::make_optional(FindResult{ .found_at = found_at, .result = text.substr(found_at, length) });
}

} // namespace zth::string
