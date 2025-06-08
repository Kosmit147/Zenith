#pragma once

#include "zenith/stl/string.hpp"
#include "zenith/util/optional.hpp"

#define MAGIC_ENUM_USING_ALIAS_STRING using string = ::zth::String;
#define MAGIC_ENUM_USING_ALIAS_STRING_VIEW using string_view = ::zth::StringView;
#define MAGIC_ENUM_USING_ALIAS_OPTIONAL template<typename T> using optional = ::zth::Optional<T>;

// We want to enforce setting the range manually in order to speed up compilation times.
#define MAGIC_ENUM_RANGE_MIN 0
#define MAGIC_ENUM_RANGE_MAX 1
