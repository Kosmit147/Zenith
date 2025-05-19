#pragma once

#include <entt/container/dense_set.hpp>

#include <functional>
#include <set>
#include <unordered_set>

#include "zenith/memory/alloc.hpp"
#include "zenith/stl/hash.hpp"

namespace zth {

template<typename Key, typename Compare = std::less<>, memory::Allocator Allocator = memory::DefaultAllocator<Key>>
using Set = std::set<Key, Compare, Allocator>;

template<typename Key, typename Compare = std::less<>, memory::Allocator Allocator = memory::DefaultAllocator<Key>>
using Multiset = std::multiset<Key, Compare, Allocator>;

template<typename Key, typename Hasher = Hash<Key>, typename KeyEqual = std::equal_to<>,
         memory::Allocator Allocator = memory::DefaultAllocator<Key>>
using UnorderedSet = std::unordered_set<Key, Hasher, KeyEqual, Allocator>;

template<typename Key, typename Hasher = Hash<Key>, typename KeyEqual = std::equal_to<>,
         memory::Allocator Allocator = memory::DefaultAllocator<Key>>
using UnorderedMultiset = std::unordered_multiset<Key, Hasher, KeyEqual, Allocator>;

template<typename Key, typename Hasher = Hash<Key>, typename KeyEqual = std::equal_to<>,
         memory::Allocator Allocator = memory::DefaultAllocator<Key>>
using DenseUnorderedSet = entt::dense_set<Key, Hasher, KeyEqual, Allocator>;

} // namespace zth
