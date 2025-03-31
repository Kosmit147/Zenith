#include "zenith/core/scene_manager.hpp"

#include "zenith/core/assert.hpp"
#include "zenith/core/scene.hpp"
#include "zenith/log/logger.hpp"
#include "zenith/renderer/renderer.hpp"

namespace zth {

auto SceneManager::init() -> Result<void, String>
{
    ZTH_CORE_INFO("Scene manager initialized.");
    return {};
}

auto SceneManager::start_frame() -> void
{
    if (_queued_scene)
    {
        if (_scene)
            _scene->on_unload();

        _scene = std::move(_queued_scene);
        _scene->on_load();
    }

    if (!_scene)
        return;

    _scene->start_frame();
}

auto SceneManager::dispatch_event(const Event& event) -> void
{
    if (!_scene)
        return;

    _scene->dispatch_event(event);
}

auto SceneManager::update() -> void
{
    if (!_scene)
    {
        ZTH_CORE_WARN("[Scene Manager] No scene loaded.");
        return;
    }

    _scene->update();
}

auto SceneManager::render() -> void
{
    if (!_scene)
        return;

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

auto SceneManager::queue_scene(std::unique_ptr<Scene>&& scene) -> void
{
    _queued_scene = std::move(scene);
}

auto SceneManager::scene() -> Optional<Reference<Scene>>
{
    if (!_scene)
        return nil;

    return *_scene;
}

auto SceneManager::scene_unchecked() -> Scene&
{
    ZTH_ASSERT(_scene != nullptr);
    return *_scene;
}

} // namespace zth
