#pragma once

#include <entt/container/dense_map.hpp>

#include <memory>
#include <type_traits>

#include "zenith/core/typedefs.hpp"
#include "zenith/gl/fwd.hpp"
#include "zenith/renderer/fwd.hpp"
#include "zenith/stl/string.hpp"
#include "zenith/util/optional.hpp"
#include "zenith/util/reference.hpp"
#include "zenith/util/result.hpp"

namespace zth {

using AssetId = u32;

// clang-format off

template<typename T> struct is_asset : std::false_type {};
template<typename T> constexpr auto is_asset_v = is_asset<T>::value;

template<> struct is_asset<Mesh> : std::true_type {};
template<> struct is_asset<Material> : std::true_type {};
template<> struct is_asset<gl::Shader> : std::true_type {};
template<> struct is_asset<gl::Texture2D> : std::true_type {};

// clang-format on

template<typename T>
concept Asset = is_asset_v<T>;

class AssetManager
{
public:
    template<Asset A> using AssetStorage = entt::dense_map<AssetId, std::shared_ptr<A>>;

    AssetManager() = delete;

    [[nodiscard]] static auto init() -> Result<void, String>;
    static auto shut_down() -> void;

    template<Asset A> static auto emplace(AssetId id, auto&&... args) -> Optional<Reference<std::shared_ptr<A>>>;

    template<Asset A>
    static auto add(AssetId id, const std::shared_ptr<A>& handle) -> Optional<Reference<std::shared_ptr<A>>>;

    template<Asset A>
    static auto add(AssetId id, std::shared_ptr<A>&& handle) -> Optional<Reference<std::shared_ptr<A>>>;

    template<Asset A>
        requires(!std::is_reference_v<A>)
    static auto add(AssetId id, const A& asset) -> Optional<Reference<std::shared_ptr<A>>>;

    template<Asset A>
        requires(!std::is_reference_v<A>)
    static auto add(AssetId id, A&& asset) -> Optional<Reference<std::shared_ptr<A>>>;

    template<Asset A> [[nodiscard]] static auto get(AssetId id) -> Optional<std::shared_ptr<A>>;
    template<Asset A> [[nodiscard]] static auto get_unchecked(AssetId id) -> std::shared_ptr<A>;

    template<Asset A> static auto remove(AssetId id) -> bool;

    template<Asset A> [[nodiscard]] static auto contains(AssetId id) -> bool;

private:
    template<Asset A> static AssetStorage<A> _storage;
    template<Asset A> static StringView _asset_type_str;
};

} // namespace zth

#include "asset.inl"
