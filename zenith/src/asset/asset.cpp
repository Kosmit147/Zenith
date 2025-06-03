#include "zenith/asset/asset.hpp"

#include "zenith/gl/shader.hpp"
#include "zenith/gl/texture.hpp"
#include "zenith/log/logger.hpp"
#include "zenith/renderer/material.hpp"
#include "zenith/renderer/mesh.hpp"

namespace zth {

template<> AssetManager::AssetStorage<Mesh> AssetManager::_storage<Mesh>;
template<> AssetManager::AssetStorage<Material> AssetManager::_storage<Material>;
template<> AssetManager::AssetStorage<gl::Shader> AssetManager::_storage<gl::Shader>;
template<> AssetManager::AssetStorage<gl::Texture2D> AssetManager::_storage<gl::Texture2D>;

template<> StringView AssetManager::_asset_type_string<Mesh> = "mesh";
template<> StringView AssetManager::_asset_type_string<Material> = "material";
template<> StringView AssetManager::_asset_type_string<gl::Shader> = "shader";
template<> StringView AssetManager::_asset_type_string<gl::Texture2D> = "texture";

auto AssetManager::init() -> Result<void, String>
{
    ZTH_INTERNAL_TRACE("Asset manager initialized.");
    return {};
}

auto AssetManager::shut_down() -> void
{
    ZTH_INTERNAL_TRACE("Shutting down asset manager...");

    _storage<Mesh>.clear();
    _storage<Material>.clear();
    _storage<gl::Shader>.clear();
    _storage<gl::Texture2D>.clear();

    ZTH_INTERNAL_TRACE("Asset manager shut down.");
}

} // namespace zth
