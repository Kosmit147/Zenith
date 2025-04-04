#include "zenith/asset/asset.hpp"

#include "zenith/gl/shader.hpp"
#include "zenith/gl/texture.hpp"
#include "zenith/log/logger.hpp"
#include "zenith/renderer/material.hpp"

namespace zth {

AssetManager::AssetMap<Material> AssetManager::_materials;
AssetManager::AssetMap<gl::Shader> AssetManager::_shaders;
AssetManager::AssetMap<gl::Texture2D> AssetManager::_textures;

auto AssetManager::init() -> Result<void, String>
{
    ZTH_CORE_INFO("Asset manager initialized.");
    return {};
}

auto AssetManager::shut_down() -> void
{
    ZTH_CORE_INFO("Shutting down asset manager...");
    _materials.clear();
    _shaders.clear();
    _textures.clear();
    ZTH_CORE_INFO("Asset manager shut down.");
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
