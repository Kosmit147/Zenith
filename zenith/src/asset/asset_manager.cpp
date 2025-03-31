#include "zenith/asset/asset_manager.hpp"

#include "zenith/core/assert.hpp"
#include "zenith/fs/file.hpp"
#include "zenith/gl/shader.hpp"
#include "zenith/log/logger.hpp"
#include "zenith/renderer/material.hpp"

namespace zth {

StringHashMap<gl::Shader> AssetManager::_shaders;
StringHashMap<gl::Texture2D> AssetManager::_textures;
StringHashMap<Material> AssetManager::_materials;

auto AssetManager::init() -> Result<void, String>
{
    ZTH_CORE_INFO("Asset manager initialized.");
    return {};
}

auto AssetManager::shut_down() -> void
{
    ZTH_CORE_INFO("Shutting down asset manager...");

    _shaders.clear();
    _textures.clear();
    _materials.clear();

    ZTH_CORE_INFO("Asset manager shut down.");
}

// @cleanup: Could probably remove some code duplication by making all of these functions templated on the asset type.

auto AssetManager::add_shader(StringView identifier, gl::Shader&& shader) -> Optional<Reference<gl::Shader>>
{
    auto [kv, success] = _shaders.emplace(identifier, std::move(shader));

    if (!success)
    {
        ZTH_CORE_ERROR("[Asset Manager] Couldn't add shader \"{}\".", identifier);
        return nil;
    }

    auto& [_, shader_ref] = *kv;
    return shader_ref;
}

auto AssetManager::add_shader_from_sources(StringView identifier, const gl::ShaderSources& sources)
    -> Optional<Reference<gl::Shader>>
{
    auto [kv, success] = _shaders.try_emplace(String{ identifier }, sources);

    if (!success)
    {
        ZTH_CORE_ERROR("[Asset Manager] Couldn't add shader \"{}\" from sources.", identifier);
        return nil;
    }

    auto& [_, shader_ref] = *kv;
    return shader_ref;
}

auto AssetManager::add_shader_from_files(StringView identifier, const gl::ShaderSourcePaths& paths)
    -> Optional<Reference<gl::Shader>>
{
    auto [kv, success] = _shaders.try_emplace(String{ identifier }, paths);

    if (!success)
    {
        ZTH_CORE_ERROR("[Asset Manager] Couldn't add shader \"{}\" from files {}.", identifier, paths);
        return nil;
    }

    auto& [_, shader_ref] = *kv;
    return shader_ref;
}

auto AssetManager::add_texture(StringView identifier, gl::Texture2D&& texture) -> Optional<Reference<gl::Texture2D>>
{
    auto [kv, success] = _textures.emplace(identifier, std::move(texture));

    if (!success)
    {
        ZTH_CORE_ERROR("[Asset Manager] Couldn't add texture \"{}\".", identifier);
        return nil;
    }

    auto& [_, texture_ref] = *kv;
    return texture_ref;
}

auto AssetManager::add_texture_from_memory(StringView identifier, const void* data, usize data_size_bytes,
                                           const gl::TextureParams& params) -> Optional<Reference<gl::Texture2D>>
{
    auto [kv, success] = _textures.try_emplace(String{ identifier }, data, data_size_bytes, params);

    if (!success)
    {
        ZTH_CORE_ERROR("[Asset Manager] Couldn't add texture \"{}\" from memory.", identifier);
        return nil;
    }

    auto& [_, texture_ref] = *kv;
    return texture_ref;
}

auto AssetManager::add_texture_from_file(const std::filesystem::path& path, const gl::TextureParams& params)
    -> Optional<Reference<gl::Texture2D>>
{
    auto filename = fs::extract_filename(path);

    if (!filename)
    {
        // @robustness: .string() throws.
        ZTH_CORE_ERROR("[Asset Manager] Couldn't add texture from file \"{}\".", path.string());
        return nil;
    }

    return add_texture_from_file(*filename, path, params);
}

auto AssetManager::add_texture_from_file(StringView identifier, const std::filesystem::path& path,
                                         const gl::TextureParams& params) -> Optional<Reference<gl::Texture2D>>
{
    auto [kv, success] = _textures.try_emplace(String{ identifier }, path, params);

    if (!success)
    {
        // @robustness: .string() throws.
        ZTH_CORE_ERROR("[Asset Manager] Couldn't add texture \"{}\" from file {}.", identifier, path.string());
        return nil;
    }

    auto& [_, texture_ref] = *kv;
    return texture_ref;
}

auto AssetManager::add_material(StringView identifier, const Material& material) -> Optional<Reference<Material>>
{
    auto [kv, success] = _materials.emplace(identifier, material);

    if (!success)
    {
        ZTH_CORE_ERROR("[Asset Manager] Couldn't add material \"{}\".", identifier);
        return nil;
    }

    auto& [_, material_ref] = *kv;
    return material_ref;
}

auto AssetManager::add_material(StringView identifier, Material&& material) -> Optional<Reference<Material>>
{
    auto [kv, success] = _materials.emplace(identifier, std::move(material));

    if (!success)
    {
        ZTH_CORE_ERROR("[Asset Manager] Couldn't add material \"{}\".", identifier);
        return nil;
    }

    auto& [_, material_ref] = *kv;
    return material_ref;
}

auto AssetManager::get_shader(StringView identifier) -> Optional<Reference<gl::Shader>>
{
    if (auto kv = _shaders.find(identifier); kv != _shaders.end())
    {
        auto& [_, shader_ref] = *kv;
        return shader_ref;
    }

    ZTH_CORE_ERROR("[Asset Manager] Couldn't get shader \"{}\".", identifier);
    return nil;
}

auto AssetManager::get_texture(StringView identifier) -> Optional<Reference<gl::Texture2D>>
{
    if (auto kv = _textures.find(identifier); kv != _textures.end())
    {
        auto& [_, texture_ref] = *kv;
        return texture_ref;
    }

    ZTH_CORE_ERROR("[Asset Manager] Couldn't get texture \"{}\".", identifier);
    return nil;
}

auto AssetManager::get_material(StringView identifier) -> Optional<Reference<Material>>
{
    if (auto kv = _materials.find(identifier); kv != _materials.end())
    {
        auto& [_, texture_ref] = *kv;
        return texture_ref;
    }

    ZTH_CORE_ERROR("[Asset Manager] Couldn't get material \"{}\".", identifier);
    return nil;
}

auto AssetManager::get_shader_unchecked(StringView identifier) -> gl::Shader&
{
    auto kv = _shaders.find(identifier);
    ZTH_ASSERT(kv != _shaders.end());
    auto& [_, shader_ref] = *kv;
    return shader_ref;
}

auto AssetManager::get_texture_unchecked(StringView identifier) -> gl::Texture2D&
{
    auto kv = _textures.find(identifier);
    ZTH_ASSERT(kv != _textures.end());
    auto& [_, texture_ref] = *kv;
    return texture_ref;
}

auto AssetManager::get_material_unchecked(StringView identifier) -> Material&
{
    auto kv = _materials.find(identifier);
    ZTH_ASSERT(kv != _materials.end());
    auto& [_, material_ref] = *kv;
    return material_ref;
}

auto AssetManager::remove_shader(StringView identifier) -> bool
{
    auto elems_erased = _shaders.erase(identifier);
    return elems_erased != 0;
}

auto AssetManager::remove_texture(StringView identifier) -> bool
{
    auto elems_erased = _textures.erase(identifier);
    return elems_erased != 0;
}

auto AssetManager::remove_material(StringView identifier) -> bool
{
    auto elems_erased = _materials.erase(identifier);
    return elems_erased != 0;
}

} // namespace zth
