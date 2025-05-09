#pragma once

#include <expected>

namespace zth {

template<typename T, typename E> using Result = std::expected<T, E>;
template<typename E> using Error = std::unexpected<E>;

} // namespace zth
