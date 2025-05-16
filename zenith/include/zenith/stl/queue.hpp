#pragma once

#include <concepts>
#include <queue>
#include <utility>

#include "zenith/stl/deque.hpp"

namespace zth {

template<typename Container>
concept AdaptableIntoQueue =
    requires(Container c, typename Container::value_type& lv, typename Container::value_type&& rv) {
        { c.back() } -> std::same_as<typename Container::value_type&>;
        { c.front() } -> std::same_as<typename Container::value_type&>;
        c.push_back(lv);
        c.push_back(std::move(rv));
        c.pop_front();
    };

template<std::movable T, AdaptableIntoQueue Container = Deque<T>> using Queue = std::queue<T, Container>;

} // namespace zth
