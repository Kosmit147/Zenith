#pragma once

#include <entt/container/dense_map.hpp>

#include <functional>
#include <map>
#include <unordered_map>
#include <utility>

#include "zenith/memory/alloc.hpp"
#include "zenith/stl/hash.hpp"

namespace zth {

template<typename Key, typename T, typename Compare = std::less<>,
         memory::Allocator Allocator = memory::DefaultAllocator<std::pair<const Key, T>>>
using Map = std::map<Key, T, Compare, Allocator>;

template<typename Key, typename T, typename Compare = std::less<>,
         memory::Allocator Allocator = memory::DefaultAllocator<std::pair<const Key, T>>>
using Multimap = std::multimap<Key, T, Compare, Allocator>;

template<typename Key, typename T, typename Hasher = Hash<Key>, typename KeyEqual = std::equal_to<>,
         memory::Allocator Allocator = memory::DefaultAllocator<std::pair<const Key, T>>>
using UnorderedMap = std::unordered_map<Key, T, Hasher, KeyEqual, Allocator>;

template<typename Key, typename T, typename Hasher = Hash<Key>, typename KeyEqual = std::equal_to<>,
         memory::Allocator Allocator = memory::DefaultAllocator<std::pair<const Key, T>>>
using UnorderedMultimap = std::unordered_multimap<Key, T, Hasher, KeyEqual, Allocator>;

template<typename Key, typename T, typename Hasher = Hash<Key>, typename KeyEqual = std::equal_to<>,
         memory::Allocator Allocator = memory::DefaultAllocator<std::pair<const Key, T>>>
using DenseUnorderedMap = entt::dense_map<Key, T, Hasher, KeyEqual, Allocator>;

} // namespace zth
