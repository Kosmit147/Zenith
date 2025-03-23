#include "zenith/core/scene_manager.hpp"

#include "zenith/core/scene.hpp"
#include "zenith/log/logger.hpp"
#include "zenith/renderer/renderer.hpp"

namespace zth {

auto SceneManager::init() -> void
{
    ZTH_CORE_INFO("Scene manager initialized.");
}

auto SceneManager::dispatch_event(const Event& event) -> void
{
    if (!_scene)
        return;

    _scene->dispatch_event(event);
}

auto SceneManager::update() -> void
{
    if (_queued_scene)
    {
        if (_scene)
            _scene->on_unload();

        _scene = std::move(_queued_scene);
        _scene->on_load();
    }

    if (!_scene)
    {
        ZTH_CORE_WARN("[Scene Manager] No scene loaded.");
        return;
    }

    _scene->update();
}

auto SceneManager::render_scene() -> void
{
    _scene->render();
}

auto SceneManager::on_render() -> void
{
    if (!_scene)
        return;

    _scene->on_render();
}

auto SceneManager::shut_down() -> void
{
    ZTH_CORE_INFO("Shutting down scene manager...");

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
