#pragma once

#include <utility>

#include "zenith/core/assert.hpp"
#include "zenith/log/logger.hpp"

namespace zth {

template<Asset A> constexpr auto get_asset_type_string() -> const char*
{
    static_assert(false, "not implemented");
}

template<> constexpr auto get_asset_type_string<Material>() -> const char*
{
    return "material";
}

template<> constexpr auto get_asset_type_string<gl::Shader>() -> const char*
{
    return "shader";
}

template<> constexpr auto get_asset_type_string<gl::Texture2D>() -> const char*
{
    return "texture";
}

template<Asset A> auto AssetManager::emplace(StringView identifier, auto&&... args) -> Optional<Reference<A>>
{
    auto& map = get_asset_map<A>();
    auto [kv, success] = map.try_emplace(String{ identifier }, std::forward<decltype(args)>(args)...);

    if (!success)
    {
        ZTH_INTERNAL_ERROR("[Asset Manager] Couldn't emplace {} \"{}\".", get_asset_type_string<A>(), identifier);
        return nil;
    }

    auto& [_, asset_ref] = *kv;
    return asset_ref;
}

template<Asset A>
    requires(!std::is_reference_v<A>)
auto AssetManager::add(StringView identifier, const A& asset) -> Optional<Reference<A>>
{
    static_assert(std::is_lvalue_reference_v<decltype(asset)>);

    auto& map = get_asset_map<A>();
    auto [kv, success] = map.emplace(identifier, asset);

    if (!success)
    {
        ZTH_INTERNAL_ERROR("[Asset Manager] Couldn't add {} \"{}\".", get_asset_type_string<A>(), identifier);
        return nil;
    }

    auto& [_, asset_ref] = *kv;
    return asset_ref;
}

template<Asset A>
    requires(!std::is_reference_v<A>)
auto AssetManager::add(StringView identifier, A&& asset) -> Optional<Reference<A>>
{
    static_assert(std::is_rvalue_reference_v<decltype(asset)>);

    auto& map = get_asset_map<A>();
    auto [kv, success] = map.emplace(identifier, std::move(asset));

    if (!success)
    {
        ZTH_INTERNAL_ERROR("[Asset Manager] Couldn't add {} \"{}\".", get_asset_type_string<A>(), identifier);
        return nil;
    }

    auto& [_, asset_ref] = *kv;
    return asset_ref;
}

template<Asset A> auto AssetManager::get(StringView identifier) -> Optional<Reference<A>>
{
    auto& map = get_asset_map<A>();

    if (auto kv = map.find(identifier); kv != map.end())
    {
        auto& [_, asset_ref] = *kv;
        return asset_ref;
    }

    ZTH_INTERNAL_ERROR("[Asset Manager] Couldn't get {} \"{}\".", get_asset_type_string<A>(), identifier);
    return nil;
}

template<Asset A> auto AssetManager::get_unchecked(StringView identifier) -> A&
{
    auto& map = get_asset_map<A>();
    auto kv = map.find(identifier);
    ZTH_ASSERT(kv != map.end());
    auto& [_, asset_ref] = *kv;
    return asset_ref;
}

template<Asset A> auto AssetManager::remove(StringView identifier) -> bool
{
    auto& map = get_asset_map<A>();
    auto elems_erased = map.erase(identifier);
    return elems_erased != 0;
}

template<Asset A> auto AssetManager::get_asset_map() -> AssetMap<A>&
{
    static_assert(false, "type is not an asset");
}

} // namespace zth
