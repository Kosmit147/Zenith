#pragma once

#include <string>
#include <unordered_map>
#include <utility>

#include "Zenith/Utility/StringHasher.hpp"

namespace zth {

template<typename ValueType>
using StringHashMap = std::unordered_map<std::string, ValueType, StringHasher, std::equal_to<>>;

} // namespace zth
