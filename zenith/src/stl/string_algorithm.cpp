#include "zenith/stl/string_algorithm.hpp"

namespace zth {

auto substr_between(StringView text, char delim) -> Optional<StringView>
{
    return substr_between(text, delim, delim);
}

auto substr_between(StringView text, char opening_delim, char closing_delim) -> Optional<StringView>
{
    return substr_between_at_offset(text, 0, opening_delim, closing_delim);
}

auto substr_between_at_offset(StringView text, usize offset, char delim) -> Optional<StringView>
{
    return substr_between_at_offset(text, offset, delim, delim);
}

auto substr_between_at_offset(StringView text, usize offset, char opening_delim, char closing_delim)
    -> Optional<StringView>
{
    auto start = text.find(opening_delim, offset);

    if (start == StringView::npos)
        return nil;

    auto end = text.find(closing_delim, start + 1);

    if (end == StringView::npos)
        return nil;

    auto found_at = start + 1;
    auto length = end - start - 1;
    return text.substr(found_at, length);
}

auto find_substr_between(StringView text, char delim) -> Optional<FindSubstrResult>
{
    return find_substr_between(text, delim, delim);
}

auto find_substr_between(StringView text, char opening_delim, char closing_delim) -> Optional<FindSubstrResult>
{
    return find_substr_between_at_offset(text, 0, opening_delim, closing_delim);
}

auto find_substr_between_at_offset(StringView text, usize offset, char delim) -> Optional<FindSubstrResult>
{
    return find_substr_between_at_offset(text, offset, delim, delim);
}

auto find_substr_between_at_offset(StringView text, usize offset, char opening_delim, char closing_delim)
    -> Optional<FindSubstrResult>
{
    auto start = text.find(opening_delim, offset);

    if (start == StringView::npos)
        return nil;

    auto end = text.find(closing_delim, start + 1);

    if (end == StringView::npos)
        return nil;

    auto found_at = start + 1;
    auto length = end - start - 1;
    return FindSubstrResult{ .found_at = found_at, .result = text.substr(found_at, length) };
}

auto case_insensitive_find(StringView text, StringView search) -> Optional<usize>
{
    auto found = std::ranges::search(text, search, case_insensitive_equal);

    if (found.empty())
        return nil;

    return std::distance(text.begin(), found.begin());
}

auto case_insensitive_contains(StringView text, StringView search) -> bool
{
    return case_insensitive_find(text, search).has_value();
}

} // namespace zth

ZTH_DEFINE_FORMATTER(zth::FindSubstrResult, result)
{
    return ZTH_FORMAT_OUT("FindSubstrResult {{\n"
                          "\t.found_at = {},\n"
                          "\t.result = {},\n"
                          "}}",
                          result.found_at, result.result);
}
