#pragma once

#include <functional>
#include <map>
#include <string>
#include <unordered_map>

#include "zenith/stl/string_hash.hpp"

namespace zth {

// A map that uses string as a key with transparent lookup.
template<typename ValueType> using StringMap = std::map<std::string, ValueType, std::less<>>;

// A hash map that uses string as a key with transparent lookup.
template<typename ValueType>
using StringHashMap = std::unordered_map<std::string, ValueType, StringHasher, std::equal_to<>>;

} // namespace zth
