#pragma once

#include <concepts>
#include <stack>
#include <utility>

#include "zenith/stl/vector.hpp"

namespace zth {

template<typename Container>
concept AdaptableIntoStack =
    requires(Container c, typename Container::value_type& lv, typename Container::value_type&& rv) {
        { c.back() } -> std::same_as<typename Container::value_type&>;
        c.push_back(lv);
        c.push_back(std::move(rv));
        c.pop_back();
    };

template<std::movable T, AdaptableIntoStack Container = Vector<T>> using Stack = std::stack<T, Container>;

} // namespace zth
