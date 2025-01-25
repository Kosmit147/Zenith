#include "zenith/core/scene_manager.hpp"

#include "zenith/core/scene.hpp"
#include "zenith/logging/logger.hpp"

namespace zth {

auto SceneManager::init() -> void
{
    ZTH_CORE_INFO("Scene manager initialized.");
}

auto SceneManager::on_update() -> void
{
    if (_queued_scene)
    {
        _scene = std::move(_queued_scene);
        _scene->on_load();
    }

    if (!_scene) [[unlikely]]
    {
        ZTH_CORE_WARN("[Scene Manager] No scene loaded.");
        return;
    }

    _scene->on_update();
}

auto SceneManager::on_event(const Event& event) -> void
{
    if (!_scene) [[unlikely]]
    {
        ZTH_CORE_WARN("[Scene Manager] No scene loaded.");
        return;
    }

    _scene->on_event(event);
}

auto SceneManager::on_render() -> void
{
    if (!_scene) [[unlikely]]
    {
        ZTH_CORE_WARN("[Scene Manager] No scene loaded.");
        return;
    }

    _scene->on_render();
}

auto SceneManager::shut_down() -> void
{
    ZTH_CORE_INFO("Scene manager shut down.");
}

auto SceneManager::load_scene(std::unique_ptr<Scene>&& scene) -> void
{
    if (!_scene)
    {
        _scene = std::move(scene);
        _scene->on_load();
    }
    else
    {
        _queued_scene = std::move(scene);
    }
}

} // namespace zth
