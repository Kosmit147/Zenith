#pragma once

#include <type_traits>
#include <unordered_map>
#include <utility>

#include "zenith/core/typedefs.hpp"
#include "zenith/gl/fwd.hpp"
#include "zenith/renderer/fwd.hpp"
#include "zenith/stl/string.hpp"
#include "zenith/util/hashed_string.hpp"
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
    template<Asset A> using AssetMap = std::unordered_map<AssetId, A>;

    AssetManager() = delete;

    [[nodiscard]] static auto init() -> Result<void, String>;
    static auto shut_down() -> void;

    template<Asset A> [[nodiscard]] static auto emplace(auto&&... args) -> Optional<std::pair<AssetId, A&>>;

    template<Asset A> static auto emplace(AssetId id, auto&&... args) -> Optional<Reference<A>>;

    template<Asset A> static auto emplace(HashedString id, auto&&... args) -> Optional<Reference<A>>;

    template<Asset A>
        requires(!std::is_reference_v<A>)
    [[nodiscard]] static auto add(const A& asset) -> Optional<std::pair<AssetId, A&>>;

    template<Asset A>
        requires(!std::is_reference_v<A>)
    [[nodiscard]] static auto add(A&& asset) -> Optional<std::pair<AssetId, A&>>;

    template<Asset A>
        requires(!std::is_reference_v<A>)
    static auto add(AssetId id, const A& asset) -> Optional<Reference<A>>;

    template<Asset A>
        requires(!std::is_reference_v<A>)
    static auto add(AssetId id, A&& asset) -> Optional<Reference<A>>;

    template<Asset A>
        requires(!std::is_reference_v<A>)
    static auto add(HashedString id, const A& asset) -> Optional<Reference<A>>;

    template<Asset A>
        requires(!std::is_reference_v<A>)
    static auto add(HashedString id, A&& asset) -> Optional<Reference<A>>;

    template<Asset A> [[nodiscard]] static auto get(AssetId id) -> Optional<Reference<A>>;
    template<Asset A> [[nodiscard]] static auto get_unchecked(AssetId id) -> A&;

    template<Asset A> static auto remove(AssetId id) -> bool;

    template<Asset A> [[nodiscard]] static auto contains(AssetId id) -> bool;

private:
    template<Asset A> static AssetId _next_id;
    // @speed: We could track free ids for reuse.

    static AssetMap<Mesh> _meshes;
    static AssetMap<Material> _materials;

    static AssetMap<gl::Shader> _shaders;
    static AssetMap<gl::Texture2D> _textures;

private:
    template<Asset A> [[nodiscard]] static auto generate_id() -> AssetId;
    template<Asset A> [[nodiscard]] static auto get_asset_map() -> AssetMap<A>&;
};

template<> auto AssetManager::get_asset_map<Mesh>() -> AssetMap<Mesh>&;
template<> auto AssetManager::get_asset_map<Material>() -> AssetMap<Material>&;

template<> auto AssetManager::get_asset_map<gl::Shader>() -> AssetMap<gl::Shader>&;
template<> auto AssetManager::get_asset_map<gl::Texture2D>() -> AssetMap<gl::Texture2D>&;

} // namespace zth

#include "asset.inl"
