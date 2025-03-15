#include "zenith/stl/string.hpp"

namespace zth::string {

auto substr_between(std::string_view text, char delim) -> Optional<std::string_view>
{
    return substr_between(text, delim, delim);
}

auto substr_between(std::string_view text, char opening_delim, char closing_delim) -> Optional<std::string_view>
{
    return substr_between_with_offset(text, 0, opening_delim, closing_delim);
}

auto substr_between_with_offset(std::string_view text, usize offset, char delim) -> Optional<std::string_view>
{
    return substr_between_with_offset(text, offset, delim, delim);
}

auto substr_between_with_offset(std::string_view text, usize offset, char opening_delim, char closing_delim)
    -> Optional<std::string_view>
{
    auto start = text.find(opening_delim, offset);

    if (start == std::string_view::npos)
        return nil;

    auto end = text.find(closing_delim, start + 1);

    if (end == std::string_view::npos)
        return nil;

    auto found_at = start + 1;
    auto length = end - start - 1;
    return text.substr(found_at, length);
}

auto find_substr_between(std::string_view text, char delim) -> Optional<FindResult>
{
    return find_substr_between(text, delim, delim);
}

auto find_substr_between(std::string_view text, char opening_delim, char closing_delim) -> Optional<FindResult>
{
    return find_substr_between_with_offset(text, 0, opening_delim, closing_delim);
}

auto find_substr_between_with_offset(std::string_view text, usize offset, char delim) -> Optional<FindResult>
{
    return find_substr_between_with_offset(text, offset, delim, delim);
}

auto find_substr_between_with_offset(std::string_view text, usize offset, char opening_delim, char closing_delim)
    -> Optional<FindResult>
{
    auto start = text.find(opening_delim, offset);

    if (start == std::string_view::npos)
        return nil;

    auto end = text.find(closing_delim, start + 1);

    if (end == std::string_view::npos)
        return nil;

    auto found_at = start + 1;
    auto length = end - start - 1;
    return FindResult{ .found_at = found_at, .result = text.substr(found_at, length) };
}

} // namespace zth::string

ZTH_DEFINE_FORMATTER(zth::string::FindResult, result)
{
    ZTH_FORMAT_OUT("{{ .found_at = {}, .result = {} }}", result.found_at, result.result);
}
