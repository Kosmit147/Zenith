#include "zenith/core/scene.hpp"

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

auto Scene::update() -> void
{
    auto scripts = _registry.view<ScriptComponent>();

    for (auto&& [entity_id, script] : scripts.each())
        script.script().on_update(EntityHandle{ entity_id, _registry });

    on_update();
}

auto Scene::render() -> void
{
    auto camera_entity_id = _registry.view<const CameraComponent>().front();

    if (camera_entity_id == null_entity)
    {
        ZTH_CORE_WARN("[Scene] No entity with a CameraComponent found.");
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

} // namespace zth
