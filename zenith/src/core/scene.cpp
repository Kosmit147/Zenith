#include "zenith/core/scene.hpp"

#include "zenith/core/assert.hpp"
#include "zenith/ecs/components.hpp"
#include "zenith/log/logger.hpp"
#include "zenith/renderer/renderer.hpp"

namespace zth {

auto Scene::start_frame() -> void
{
    on_frame_start();
}

auto Scene::dispatch_event(const Event& event) -> void
{
    auto scripts = _registry.view<ScriptComponent>();

    for (auto&& [entity_id, script] : scripts.each())
        script.script().on_event(EntityHandle{ entity_id, _registry }, event);

    on_event(event);
}

auto Scene::fixed_update() -> void
{
    auto scripts = _registry.view<ScriptComponent>();

    for (auto&& [entity_id, script] : scripts.each())
        script.script().on_fixed_update(EntityHandle{ entity_id, _registry });

    on_fixed_update();
}

auto Scene::update() -> void
{
    auto scripts = _registry.view<ScriptComponent>();

    for (auto&& [entity_id, script] : scripts.each())
        script.script().on_update(EntityHandle{ entity_id, _registry });

    on_update();

    auto marked_for_deletion = _registry.view<DeletionMarkerComponent>();

    // @speed: We could use a function which takes in either a view or a pair of iterators for destructing multiple
    // entities at once more efficiently.

    for (auto&& [entity_id] : marked_for_deletion.each())
        _registry.destroy_now_unchecked(entity_id);
}

auto Scene::render() -> void
{
    auto camera_entity_id = _registry.view<const CameraComponent>().front();

    if (camera_entity_id == null_entity)
    {
        ZTH_CORE_WARN("[Scene] No entity with a CameraComponent found.");
        Renderer::clear();
        return;
    }

    const auto& [camera, camera_transform] =
        _registry.get<const CameraComponent, const TransformComponent>(camera_entity_id);

    Renderer::begin_scene(camera, camera_transform);

    auto lights = _registry.view<const LightComponent>();

    for (auto&& [entity_id, light] : lights.each())
    {
        auto& transform = _registry.get<const TransformComponent>(entity_id);
        Renderer::submit_light(light, transform);
    }

    auto meshes = _registry.group<const TransformComponent, const MeshComponent, const MaterialComponent>();

    for (auto&& [_, transform, mesh, material] : meshes.each())
        Renderer::submit(*mesh.mesh, transform.transform(), *material.material);

    Renderer::end_scene();
}

auto Scene::create_entity(const String& tag) -> EntityHandle
{
    return _registry.create(tag);
}

auto Scene::create_entity(String&& tag) -> EntityHandle
{
    return _registry.create(std::move(tag));
}

auto Scene::find_entity_by_tag(StringView tag) -> Optional<EntityHandle>
{
    return _registry.find_entity_by_tag(tag);
}

auto Scene::find_entities_by_tag(StringView tag) -> TemporaryVector<EntityHandle>
{
    return _registry.find_entities_by_tag(tag);
}

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

auto SceneManager::fixed_update() -> void
{
    if (!_scene)
        return;

    _scene->fixed_update();
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
