#include "zenith/core/asset_manager.hpp"

#include "zenith/fs/fs.hpp"
#include "zenith/log/logger.hpp"

namespace zth {

StringHashMap<Shader> AssetManager::_shaders;

auto AssetManager::init() -> void
{
    ZTH_CORE_INFO("Asset manager initialized.");
}

auto AssetManager::shut_down() -> void
{
    _shaders.clear();
    ZTH_CORE_INFO("Asset manager shut down.");
}

auto AssetManager::load_shader_from_file(std::string_view name, const std::filesystem::path& vertex_path,
                                         const std::filesystem::path& fragment_path) -> std::optional<ShaderRef>
{
    auto vertex_source = fs::load_to_string(vertex_path);
    auto fragment_source = fs::load_to_string(fragment_path);

    if (!vertex_source)
    {
        // @speed: string() throws
        ZTH_CORE_DEBUG("[Asset Manager] Couldn't load vertex shader \"{}\" from file \"{}\"", name,
                       vertex_path.string());
        return {};
    }

    if (!fragment_source)
    {
        // @speed: string() throws
        ZTH_CORE_DEBUG("[Asset Manager] Couldn't load fragment shader \"{}\" from file \"{}\"", name,
                       fragment_path.string());
        return {};
    }

    return load_shader_from_memory(name, *vertex_source, *fragment_source);
}

auto AssetManager::load_shader_from_memory(std::string_view name, std::string_view vertex_source,
                                           std::string_view fragment_source) -> std::optional<ShaderRef>
{
    auto [kv, success] = _shaders.try_emplace(std::string{ name }, vertex_source, fragment_source);

    if (!success)
    {
        ZTH_CORE_DEBUG("[Asset Manager] Couldn't load shader \"{}\" from memory.", name);
        return {};
    }

    auto& [shader_name, shader] = *kv;
    return shader;
}

auto AssetManager::get_shader(std::string_view name) -> std::optional<ShaderRef>
{
    if (auto kv = _shaders.find(name); kv != _shaders.end())
    {
        auto& [_, shader] = *kv;
        return shader;
    }

    ZTH_CORE_DEBUG("[Asset Manager] Couldn't get shader \"{}\".", name);
    return {};
}

auto AssetManager::unload_shader(std::string_view name) -> bool
{
    auto elems_erased = _shaders.erase(name);
    return elems_erased > 0;
}

} // namespace zth
