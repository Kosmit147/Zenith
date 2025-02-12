#include "zenith/core/scene_manager.hpp"

#include "zenith/core/scene.hpp"
#include "zenith/graphics/renderer.hpp"
#include "zenith/log/logger.hpp"

namespace zth {

auto SceneManager::init() -> void
{
    ZTH_CORE_INFO("Scene manager initialized.");
}

auto SceneManager::on_update() -> void
{
    if (_queued_scene)
    {
        if (_scene)
            _scene->on_unload();

        // @temporary
        Renderer::clear_scene_data();

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
    _queued_scene.reset();

    if (_scene)
        _scene->on_unload();

    _scene.reset();

    ZTH_CORE_INFO("Scene manager shut down.");
}

auto SceneManager::load_scene(std::unique_ptr<Scene>&& scene) -> void
{
    _queued_scene = std::move(scene);
}

} // namespace zth
