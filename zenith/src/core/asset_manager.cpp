#include "zenith/core/asset_manager.hpp"

#include "zenith/fs/fs.hpp"
#include "zenith/log/logger.hpp"

namespace zth {

StringHashMap<gl::Shader> AssetManager::_shaders;

auto AssetManager::init() -> void
{
    ZTH_CORE_INFO("Asset manager initialized.");
}

auto AssetManager::shut_down() -> void
{
    _shaders.clear();
    ZTH_CORE_INFO("Asset manager shut down.");
}

auto AssetManager::add_shader(std::string_view name, gl::Shader&& shader) -> std::optional<ShaderRef>
{
    auto [kv, success] = _shaders.emplace(name, std::move(shader));

    if (!success)
    {
        ZTH_CORE_ERROR("[Asset Manager] Couldn't add shader \"{}\".", name);
        return {};
    }

    auto& [_, shader_ref] = *kv;
    return shader_ref;
}

auto AssetManager::add_shader_from_sources(std::string_view name,
                                           const gl::ShaderSources& sources) -> std::optional<ShaderRef>
{
    auto [kv, success] = _shaders.try_emplace(std::string{ name }, sources);

    if (!success)
    {
        ZTH_CORE_ERROR("[Asset Manager] Couldn't add shader \"{}\" from sources.", name);
        return {};
    }

    auto& [_, shader] = *kv;
    return shader;
}

auto AssetManager::add_shader_from_files(std::string_view name,
                                         const gl::ShaderSourcePaths& paths) -> std::optional<ShaderRef>
{
    auto [kv, success] = _shaders.try_emplace(std::string{ name }, paths);

    if (!success)
    {
        ZTH_CORE_ERROR("[Asset Manager] Couldn't add shader \"{}\" from files.", name);
        return {};
    }

    auto& [_, shader] = *kv;
    return shader;
}

auto AssetManager::get_shader(std::string_view name) -> std::optional<ShaderRef>
{
    if (auto kv = _shaders.find(name); kv != _shaders.end())
    {
        auto& [_, shader] = *kv;
        return shader;
    }

    ZTH_CORE_ERROR("[Asset Manager] Couldn't get shader \"{}\".", name);
    return {};
}

auto AssetManager::remove_shader(std::string_view name) -> bool
{
    auto elems_erased = _shaders.erase(name);
    return elems_erased > 0;
}

} // namespace zth
