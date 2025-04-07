#pragma once

#include <type_traits>

#include "zenith/gl/fwd.hpp"
#include "zenith/renderer/fwd.hpp"
#include "zenith/stl/string.hpp"
#include "zenith/stl/string_map.hpp"
#include "zenith/util/optional.hpp"
#include "zenith/util/reference.hpp"
#include "zenith/util/result.hpp"

namespace zth {

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

template<Asset A> [[nodiscard]] constexpr auto get_asset_type_string() -> const char*;

class AssetManager
{
public:
    template<Asset A> using AssetMap = StringHashMap<A>;

    AssetManager() = delete;

    [[nodiscard]] static auto init() -> Result<void, String>;
    static auto shut_down() -> void;

    template<Asset A> static auto emplace(StringView identifier, auto&&... args) -> Optional<Reference<A>>;

    template<Asset A>
        requires(!std::is_reference_v<A>)
    static auto add(StringView identifier, const A& asset) -> Optional<Reference<A>>;

    template<Asset A>
        requires(!std::is_reference_v<A>)
    static auto add(StringView identifier, A&& asset) -> Optional<Reference<A>>;

    template<Asset A> [[nodiscard]] static auto get(StringView identifier) -> Optional<Reference<A>>;
    template<Asset A> [[nodiscard]] static auto get_unchecked(StringView identifier) -> A&;
    template<Asset A> static auto remove(StringView identifier) -> bool;

private:
    static AssetMap<Mesh> _meshes;
    static AssetMap<Material> _materials;
    static AssetMap<gl::Shader> _shaders;
    static AssetMap<gl::Texture2D> _textures;

private:
    template<Asset A> [[nodiscard]] static auto get_asset_map() -> AssetMap<A>&;
};

template<> auto AssetManager::get_asset_map<Mesh>() -> AssetMap<Mesh>&;
template<> auto AssetManager::get_asset_map<Material>() -> AssetMap<Material>&;
template<> auto AssetManager::get_asset_map<gl::Shader>() -> AssetMap<gl::Shader>&;
template<> auto AssetManager::get_asset_map<gl::Texture2D>() -> AssetMap<gl::Texture2D>&;

extern template auto AssetManager::add<Mesh>(StringView, const Mesh&) -> Optional<Reference<Mesh>>;
extern template auto AssetManager::add<Mesh>(StringView, Mesh&&) -> Optional<Reference<Mesh>>;
extern template auto AssetManager::get<Mesh>(StringView) -> Optional<Reference<Mesh>>;
extern template auto AssetManager::get_unchecked<Mesh>(StringView) -> Mesh&;
extern template auto AssetManager::remove<Mesh>(StringView) -> bool;

extern template auto AssetManager::add<Material>(StringView, const Material&) -> Optional<Reference<Material>>;
extern template auto AssetManager::add<Material>(StringView, Material&&) -> Optional<Reference<Material>>;
extern template auto AssetManager::get<Material>(StringView) -> Optional<Reference<Material>>;
extern template auto AssetManager::get_unchecked<Material>(StringView) -> Material&;
extern template auto AssetManager::remove<Material>(StringView) -> bool;

extern template auto AssetManager::add<gl::Shader>(StringView, gl::Shader&&) -> Optional<Reference<gl::Shader>>;
extern template auto AssetManager::get<gl::Shader>(StringView) -> Optional<Reference<gl::Shader>>;
extern template auto AssetManager::get_unchecked<gl::Shader>(StringView) -> gl::Shader&;
extern template auto AssetManager::remove<gl::Shader>(StringView) -> bool;

extern template auto AssetManager::add<gl::Texture2D>(StringView, gl::Texture2D&&)
    -> Optional<Reference<gl::Texture2D>>;
extern template auto AssetManager::get<gl::Texture2D>(StringView) -> Optional<Reference<gl::Texture2D>>;
extern template auto AssetManager::get_unchecked<gl::Texture2D>(StringView) -> gl::Texture2D&;
extern template auto AssetManager::remove<gl::Texture2D>(StringView) -> bool;

} // namespace zth

#include "asset.inl"
