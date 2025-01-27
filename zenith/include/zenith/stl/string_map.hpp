#pragma once

#include <functional>
#include <map>
#include <string>
#include <unordered_map>

#include "zenith/stl/string_hash.hpp"

namespace zth {

// a map that uses a string as key with transparent lookup
template<typename ValueType> using StringMap = std::map<std::string, ValueType, std::less<>>;

// a hash map that uses a string as key with transparent lookup
template<typename ValueType>
using StringHashMap = std::unordered_map<std::string, ValueType, StringHasher, std::equal_to<>>;

} // namespace zth
