#include "zenith/core/asset_manager.hpp"

#include "zenith/fs/file.hpp"
#include "zenith/gl/shader.hpp"
#include "zenith/log/logger.hpp"

namespace zth {

StringHashMap<gl::Shader> AssetManager::_shaders;
StringHashMap<gl::Texture2D> AssetManager::_textures;

auto AssetManager::init() -> void
{
    ZTH_CORE_INFO("Asset manager initialized.");
}

auto AssetManager::shut_down() -> void
{
    ZTH_CORE_INFO("Shutting down asset manager...");

    _shaders.clear();
    _textures.clear();

    ZTH_CORE_INFO("Asset manager shut down.");
}

// @cleanup: Could probably remove some code duplication in add_* functions.

auto AssetManager::add_shader(StringView name, gl::Shader&& shader) -> Optional<Reference<gl::Shader>>
{
    auto [kv, success] = _shaders.emplace(name, std::move(shader));

    if (!success)
    {
        ZTH_CORE_ERROR("[Asset Manager] Couldn't add shader \"{}\".", name);
        return nil;
    }

    auto& [_, shader_ref] = *kv;
    return shader_ref;
}

auto AssetManager::add_shader_from_sources(StringView name, const gl::ShaderSources& sources)
    -> Optional<Reference<gl::Shader>>
{
    auto [kv, success] = _shaders.try_emplace(String{ name }, sources);

    if (!success)
    {
        ZTH_CORE_ERROR("[Asset Manager] Couldn't add shader \"{}\" from sources.", name);
        return nil;
    }

    auto& [_, shader] = *kv;
    return shader;
}

auto AssetManager::add_shader_from_files(StringView name, const gl::ShaderSourcePaths& paths)
    -> Optional<Reference<gl::Shader>>
{
    auto [kv, success] = _shaders.try_emplace(String{ name }, paths);

    if (!success)
    {
        ZTH_CORE_ERROR("[Asset Manager] Couldn't add shader \"{}\" from files {}.", name, paths);
        return nil;
    }

    auto& [_, shader] = *kv;
    return shader;
}

auto AssetManager::add_texture(StringView name, gl::Texture2D&& texture) -> Optional<Reference<gl::Texture2D>>
{
    auto [kv, success] = _textures.emplace(name, std::move(texture));

    if (!success)
    {
        ZTH_CORE_ERROR("[Asset Manager] Couldn't add texture \"{}\".", name);
        return nil;
    }

    auto& [_, texture_ref] = *kv;
    return texture_ref;
}

auto AssetManager::add_texture_from_memory(StringView name, const void* data, usize data_size_bytes,
                                           const gl::TextureParams& params) -> Optional<Reference<gl::Texture2D>>
{
    auto [kv, success] = _textures.try_emplace(String{ name }, data, data_size_bytes, params);

    if (!success)
    {
        ZTH_CORE_ERROR("[Asset Manager] Couldn't add texture \"{}\" from memory.", name);
        return nil;
    }

    auto& [_, texture] = *kv;
    return texture;
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

auto AssetManager::add_texture_from_file(StringView name, const std::filesystem::path& path,
                                         const gl::TextureParams& params) -> Optional<Reference<gl::Texture2D>>
{
    auto [kv, success] = _textures.try_emplace(String{ name }, path, params);

    if (!success)
    {
        // @robustness: .string() throws.
        ZTH_CORE_ERROR("[Asset Manager] Couldn't add texture \"{}\" from file {}.", name, path.string());
        return nil;
    }

    auto& [_, texture] = *kv;
    return texture;
}

auto AssetManager::get_shader(StringView name) -> Optional<Reference<gl::Shader>>
{
    if (auto kv = _shaders.find(name); kv != _shaders.end())
    {
        auto& [_, shader] = *kv;
        return shader;
    }

    ZTH_CORE_ERROR("[Asset Manager] Couldn't get shader \"{}\".", name);
    return nil;
}

auto AssetManager::get_texture(StringView name) -> Optional<Reference<gl::Texture2D>>
{
    if (auto kv = _textures.find(name); kv != _textures.end())
    {
        auto& [_, texture] = *kv;
        return texture;
    }

    ZTH_CORE_ERROR("[Asset Manager] Couldn't get texture \"{}\".", name);
    return nil;
}

auto AssetManager::remove_shader(StringView name) -> bool
{
    auto elems_erased = _shaders.erase(name);
    return elems_erased != 0;
}

auto AssetManager::remove_texture(StringView name) -> bool
{
    auto elems_erased = _textures.erase(name);
    return elems_erased != 0;
}

} // namespace zth
