#pragma once

#include "zenith/core/assert.hpp"
#include "zenith/log/logger.hpp"

namespace zth {

template<Asset A> [[nodiscard]] constexpr auto get_asset_type_string() -> const char*
{
    static_assert(false, "not implemented");
}

template<> constexpr auto get_asset_type_string<Mesh>() -> const char*
{
    return "mesh";
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

template<Asset A> auto AssetManager::emplace(auto&&... args) -> Optional<std::pair<AssetId, A&>>
{
    auto id = generate_id<A>();
    return emplace<A>(id, std::forward<decltype(args)>(args)...).transform([&](auto&& asset_ref) {
        return std::pair<AssetId, A&>(id, asset_ref);
    });
}

template<Asset A> auto AssetManager::emplace(AssetId id, auto&&... args) -> Optional<Reference<A>>
{
    auto& map = get_asset_map<A>();
    auto [kv, success] = map.try_emplace(id, std::forward<decltype(args)>(args)...);

    if (!success)
    {
        ZTH_INTERNAL_ERROR("[Asset Manager] Couldn't emplace {} {}.", get_asset_type_string<A>(), id);
        return nil;
    }

    auto& [_, asset_ref] = *kv;
    return asset_ref;
}

template<Asset A> auto AssetManager::emplace(HashedString id, auto&&... args) -> Optional<Reference<A>>
{
    return emplace<A>(static_cast<AssetId>(id), std::forward<decltype(args)>(args)...);
}

template<Asset A>
    requires(!std::is_reference_v<A>)
auto AssetManager::add(const A& asset) -> Optional<std::pair<AssetId, A&>>
{
    auto id = generate_id<A>();
    return add<A>(id, std::forward<decltype(asset)>(asset)).transform([&](auto&& asset_ref) {
        return std::pair<AssetId, A&>(id, asset_ref);
    });
}

template<Asset A>
    requires(!std::is_reference_v<A>)
auto AssetManager::add(A&& asset) -> Optional<std::pair<AssetId, A&>>
{
    auto id = generate_id<A>();
    return add<A>(id, std::forward<decltype(asset)>(asset)).transform([&](auto&& asset_ref) {
        return std::pair<AssetId, A&>(id, asset_ref);
    });
}

template<Asset A>
    requires(!std::is_reference_v<A>)
auto AssetManager::add(AssetId id, const A& asset) -> Optional<Reference<A>>
{
    static_assert(std::is_lvalue_reference_v<decltype(asset)>);

    auto& map = get_asset_map<A>();
    auto [kv, success] = map.emplace(id, asset);

    if (!success)
    {
        ZTH_INTERNAL_ERROR("[Asset Manager] Couldn't add {} {}.", get_asset_type_string<A>(), id);
        return nil;
    }

    auto& [_, asset_ref] = *kv;
    return asset_ref;
}

template<Asset A>
    requires(!std::is_reference_v<A>)
auto AssetManager::add(AssetId id, A&& asset) -> Optional<Reference<A>>
{
    static_assert(std::is_rvalue_reference_v<decltype(asset)>);

    auto& map = get_asset_map<A>();
    auto [kv, success] = map.emplace(id, std::move(asset));

    if (!success)
    {
        ZTH_INTERNAL_ERROR("[Asset Manager] Couldn't add {} {}.", get_asset_type_string<A>(), id);
        return nil;
    }

    auto& [_, asset_ref] = *kv;
    return asset_ref;
}

template<Asset A>
    requires(!std::is_reference_v<A>)
auto AssetManager::add(HashedString id, const A& asset) -> Optional<Reference<A>>
{
    return add<A>(static_cast<AssetId>(id), std::forward<decltype(asset)>(asset));
}

template<Asset A>
    requires(!std::is_reference_v<A>)
auto AssetManager::add(HashedString id, A&& asset) -> Optional<Reference<A>>
{
    return add<A>(static_cast<AssetId>(id), std::forward<decltype(asset)>(asset));
}

template<Asset A> auto AssetManager::get(AssetId id) -> Optional<Reference<A>>
{
    auto& map = get_asset_map<A>();

    if (auto kv = map.find(id); kv != map.end())
    {
        auto& [_, asset_ref] = *kv;
        return asset_ref;
    }

    ZTH_INTERNAL_ERROR("[Asset Manager] Couldn't get {} {}.", get_asset_type_string<A>(), id);
    return nil;
}

template<Asset A> auto AssetManager::get_unchecked(AssetId id) -> A&
{
    auto& map = get_asset_map<A>();
    auto kv = map.find(id);
    ZTH_ASSERT(kv != map.end());
    auto& [_, asset_ref] = *kv;
    return asset_ref;
}

template<Asset A> auto AssetManager::remove(AssetId id) -> bool
{
    auto& map = get_asset_map<A>();
    auto elems_erased = map.erase(id);
    return elems_erased != 0;
}

template<Asset A> auto AssetManager::contains(AssetId id) -> bool
{
    auto& map = get_asset_map<A>();
    return map.contains(id);
}

template<Asset A> auto AssetManager::generate_id() -> AssetId
{
    auto& map = get_asset_map<A>();

    while (true)
    {
        auto id = _next_id<A> ++;

        if (!contains<A>(id))
            return id;
    }
}

template<Asset A> auto AssetManager::get_asset_map() -> AssetMap<A>&
{
    static_assert(false, "type is not an asset");
}

} // namespace zth
