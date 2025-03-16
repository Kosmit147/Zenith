#pragma once

#include <cstddef>
#include <utility>

#include "zenith/stl/string.hpp"

namespace zth {

struct StringHasher
{
    using is_transparent = void;

    [[nodiscard]] auto operator()(const char* text) const -> std::size_t { return std::hash<StringView>{}(text); }

    [[nodiscard]] auto operator()(StringView text) const -> std::size_t { return std::hash<StringView>{}(text); }

    [[nodiscard]] auto operator()(const String& text) const -> std::size_t { return std::hash<String>{}(text); }
};

} // namespace zth
