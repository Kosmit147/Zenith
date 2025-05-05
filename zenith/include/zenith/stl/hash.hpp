#pragma once

#include <cstddef>
#include <utility>

#include "zenith/stl/string.hpp"

namespace zth {

struct StringHash
{
    using is_transparent = void;

    [[nodiscard]] auto operator()(const char* text) const -> std::size_t { return std::hash<StringView>{}(text); }
    [[nodiscard]] auto operator()(StringView text) const -> std::size_t { return std::hash<StringView>{}(text); }
    [[nodiscard]] auto operator()(const String& text) const -> std::size_t { return std::hash<String>{}(text); }
};

// Use std::hash by default.
template<typename T> struct Hash : std::hash<T>
{
};

// String specialization to enable transparent lookup in unordered containers.
// @todo: We should enable this specialization for all string types.
template<> struct Hash<String> : StringHash
{
};

} // namespace zth
