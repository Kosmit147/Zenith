#pragma once

#include <variant> // @refactor: std::monostate is defined in <utility> beginning with C++26.

namespace zth {

using Success = std::monostate;

} // namespace zth
