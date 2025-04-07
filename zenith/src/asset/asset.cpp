#include "zenith/asset/asset.hpp"

#include "zenith/gl/shader.hpp"
#include "zenith/gl/texture.hpp"
#include "zenith/log/logger.hpp"
#include "zenith/renderer/material.hpp"
#include "zenith/renderer/mesh.hpp"

namespace zth {

AssetManager::AssetMap<Mesh> AssetManager::_meshes;
AssetManager::AssetMap<Material> AssetManager::_materials;
AssetManager::AssetMap<gl::Shader> AssetManager::_shaders;
AssetManager::AssetMap<gl::Texture2D> AssetManager::_textures;

auto AssetManager::init() -> Result<void, String>
{
    ZTH_INTERNAL_INFO("Asset manager initialized.");
    return {};
}

auto AssetManager::shut_down() -> void
{
    ZTH_INTERNAL_INFO("Shutting down asset manager...");
    _meshes.clear();
    _materials.clear();
    _shaders.clear();
    _textures.clear();
    ZTH_INTERNAL_INFO("Asset manager shut down.");
}

template<> auto AssetManager::get_asset_map<Mesh>() -> AssetMap<Mesh>&
{
    return _meshes;
}

template<> auto AssetManager::get_asset_map<Material>() -> AssetMap<Material>&
{
    return _materials;
}

template<> auto AssetManager::get_asset_map<gl::Shader>() -> AssetMap<gl::Shader>&
{
    return _shaders;
}

template<> auto AssetManager::get_asset_map<gl::Texture2D>() -> AssetMap<gl::Texture2D>&
{
    return _textures;
}

template auto AssetManager::add<Mesh>(StringView, const Mesh&) -> Optional<Reference<Mesh>>;
template auto AssetManager::add<Mesh>(StringView, Mesh&&) -> Optional<Reference<Mesh>>;
template auto AssetManager::get<Mesh>(StringView) -> Optional<Reference<Mesh>>;
template auto AssetManager::get_unchecked<Mesh>(StringView) -> Mesh&;
template auto AssetManager::remove<Mesh>(StringView) -> bool;

template auto AssetManager::add<Material>(StringView, const Material&) -> Optional<Reference<Material>>;
template auto AssetManager::add<Material>(StringView, Material&&) -> Optional<Reference<Material>>;
template auto AssetManager::get<Material>(StringView) -> Optional<Reference<Material>>;
template auto AssetManager::get_unchecked<Material>(StringView) -> Material&;
template auto AssetManager::remove<Material>(StringView) -> bool;

template auto AssetManager::add<gl::Shader>(StringView, gl::Shader&&) -> Optional<Reference<gl::Shader>>;
template auto AssetManager::get<gl::Shader>(StringView) -> Optional<Reference<gl::Shader>>;
template auto AssetManager::get_unchecked<gl::Shader>(StringView) -> gl::Shader&;
template auto AssetManager::remove<gl::Shader>(StringView) -> bool;

template auto AssetManager::add<gl::Texture2D>(StringView, gl::Texture2D&&) -> Optional<Reference<gl::Texture2D>>;
template auto AssetManager::get<gl::Texture2D>(StringView) -> Optional<Reference<gl::Texture2D>>;
template auto AssetManager::get_unchecked<gl::Texture2D>(StringView) -> gl::Texture2D&;
template auto AssetManager::remove<gl::Texture2D>(StringView) -> bool;

} // namespace zth
