#pragma once

#include <utility>

#include "zenith/log/logger.hpp"

namespace zth {

template<Asset A>
auto AssetManager::emplace(AssetId id, auto&&... args) -> Optional<Reference<const std::shared_ptr<A>>>
{
    return add<A>(id, std::make_shared<A>(std::forward<decltype(args)>(args)...));
}

template<Asset A>
auto AssetManager::add(AssetId id, const std::shared_ptr<A>& handle) -> Optional<Reference<const std::shared_ptr<A>>>
{
    auto [kv, success] = _storage<A>.emplace(id, handle);

    if (!success)
    {
        ZTH_INTERNAL_ERROR("[Asset Manager] Couldn't add {} under id {}.", _asset_type_string<A>, id);
        return nil;
    }

    auto [_, ref] = *kv;
    static_assert(std::is_reference_v<decltype(ref)>);
    return ref;
}

template<Asset A>
auto AssetManager::add(AssetId id, std::shared_ptr<A>&& handle) -> Optional<Reference<const std::shared_ptr<A>>>
{
    auto [kv, success] = _storage<A>.emplace(id, std::move(handle));

    if (!success)
    {
        ZTH_INTERNAL_ERROR("[Asset Manager] Couldn't add {} under id {}.", _asset_type_string<A>, id);
        return nil;
    }

    auto [_, ref] = *kv;
    static_assert(std::is_reference_v<decltype(ref)>);
    return ref;
}

template<Asset A>
    requires(!std::is_reference_v<A>)
auto AssetManager::add(AssetId id, const A& asset) -> Optional<Reference<const std::shared_ptr<A>>>
{
    static_assert(std::is_lvalue_reference_v<decltype(asset)>);

    auto [kv, success] = _storage<A>.emplace(id, asset);

    if (!success)
    {
        ZTH_INTERNAL_ERROR("[Asset Manager] Couldn't add {} under id {}.", _asset_type_string<A>, id);
        return nil;
    }

    auto [_, ref] = *kv;
    static_assert(std::is_reference_v<decltype(ref)>);
    return ref;
}

template<Asset A>
    requires(!std::is_reference_v<A>)
auto AssetManager::add(AssetId id, A&& asset) -> Optional<Reference<const std::shared_ptr<A>>>
{
    static_assert(std::is_rvalue_reference_v<decltype(asset)>);

    auto [kv, success] = _storage<A>.emplace(id, std::move(asset));

    if (!success)
    {
        ZTH_INTERNAL_ERROR("[Asset Manager] Couldn't add {} under id {}.", _asset_type_string<A>, id);
        return nil;
    }

    auto [_, ref] = *kv;
    static_assert(std::is_reference_v<decltype(ref)>);
    return ref;
}

template<Asset A> auto AssetManager::get(AssetId id) -> Optional<std::shared_ptr<A>>
{
    if (auto kv = _storage<A>.find(id); kv != _storage<A>.end())
    {
        auto [_, ref] = *kv;
        static_assert(std::is_reference_v<decltype(ref)>);
        return ref;
    }

    ZTH_INTERNAL_ERROR("[Asset Manager] Couldn't get {} with id {}.", _asset_type_string<A>, id);
    return nil;
}

template<Asset A> auto AssetManager::get_unchecked(AssetId id) noexcept -> std::shared_ptr<A>
{
    return _storage<A>.at(id);
}

template<Asset A> auto AssetManager::remove(AssetId id) -> bool
{
    auto elems_erased = _storage<A>.erase(id);
    return elems_erased != 0;
}

template<Asset A> auto AssetManager::contains(AssetId id) -> bool
{
    return _storage<A>.contains(id);
}

template<Asset A> auto AssetManager::all() -> AssetView<A>
{
    return std::ranges::views::all(_storage<A>);
}

} // namespace zth
