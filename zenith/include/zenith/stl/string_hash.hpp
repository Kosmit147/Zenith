#pragma once

#include <cstddef>
#include <string>
#include <string_view>
#include <utility>

namespace zth {

struct StringHasher
{
    using is_transparent = void;

    [[nodiscard]] auto operator()(const char* text) const -> std::size_t { return std::hash<std::string_view>{}(text); }

    [[nodiscard]] auto operator()(std::string_view text) const -> std::size_t
    {
        return std::hash<std::string_view>{}(text);
    }

    [[nodiscard]] auto operator()(const std::string& text) const -> std::size_t
    {
        return std::hash<std::string>{}(text);
    }
};

} // namespace zth
