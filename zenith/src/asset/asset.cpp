#include "zenith/asset/asset.hpp"

#include "zenith/gl/shader.hpp"
#include "zenith/gl/texture.hpp"
#include "zenith/log/logger.hpp"
#include "zenith/renderer/material.hpp"
#include "zenith/renderer/mesh.hpp"

namespace zth {

template<> AssetId AssetManager::_next_id<Mesh> = 0;
template<> AssetId AssetManager::_next_id<Material> = 0;
template<> AssetId AssetManager::_next_id<gl::Shader> = 0;
template<> AssetId AssetManager::_next_id<gl::Texture2D> = 0;

AssetManager::AssetMap<Mesh> AssetManager::_meshes;
AssetManager::AssetMap<Material> AssetManager::_materials;
AssetManager::AssetMap<gl::Shader> AssetManager::_shaders;
AssetManager::AssetMap<gl::Texture2D> AssetManager::_textures;

auto AssetManager::init() -> Result<void, String>
{
    ZTH_INTERNAL_TRACE("Asset manager initialized.");
    return {};
}

auto AssetManager::shut_down() -> void
{
    ZTH_INTERNAL_TRACE("Shutting down asset manager...");

    _meshes.clear();
    _materials.clear();

    _shaders.clear();
    _textures.clear();

    ZTH_INTERNAL_TRACE("Asset manager shut down.");
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

} // namespace zth
