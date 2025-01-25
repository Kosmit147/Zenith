#pragma once

#include <functional>
#include <map>
#include <string>
#include <unordered_map>

#include "zenith/utility/string_hasher.hpp"

namespace zth {

template<typename ValueType> using StringMap = std::map<std::string, ValueType, std::less<>>;

template<typename ValueType>
using StringHashMap = std::unordered_map<std::string, ValueType, StringHasher, std::equal_to<>>;

} // namespace zth
