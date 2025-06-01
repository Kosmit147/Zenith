#include "zenith/core/scene.hpp"

#include "zenith/core/assert.hpp"
#include "zenith/core/profiler.hpp"
#include "zenith/ecs/components.hpp"
#include "zenith/log/logger.hpp"
#include "zenith/renderer/coordinate_space.hpp"
#include "zenith/renderer/renderer.hpp"

namespace zth {

UniquePtr<Scene> SceneManager::_scene = nullptr;
std::function<UniquePtr<Scene>()> SceneManager::_queued_scene_factory;

Scene::Scene(const String& name) : _name{ name }
{
    set_up_registry_listeners();
}

Scene::Scene(String&& name) : _name{ std::move(name) }
{
    set_up_registry_listeners();
}

auto Scene::start_frame() -> void
{
    ZTH_PROFILE_FUNCTION();

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
    ZTH_PROFILE_FUNCTION();

    auto scripts = _registry.view<ScriptComponent>();

    for (auto&& [entity_id, script] : scripts.each())
        script.script().on_fixed_update(EntityHandle{ entity_id, _registry });

    on_fixed_update();
}

auto Scene::update() -> void
{
    ZTH_PROFILE_FUNCTION();

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
    ZTH_PROFILE_FUNCTION();

    auto camera_entity_id = _registry.view<const CameraComponent>().front();

    if (camera_entity_id == null_entity)
    {
        ZTH_INTERNAL_WARN("[Scene] No entity with a camera component found.");
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

    auto meshes = _registry.group<const MeshRendererComponent>(
        GetComponents<const TransformComponent, const MaterialComponent>{});

    for (auto&& [_, mesh, transform, material] : meshes.each())
    {
        auto& mesh_ptr = mesh.mesh();
        auto& material_ptr = material.material();
        ZTH_ASSERT(mesh_ptr != nullptr);
        ZTH_ASSERT(material_ptr != nullptr);
        Renderer::submit(*mesh_ptr, transform.transform(), *material_ptr);
    }

    Renderer::end_scene();

    Renderer2D::begin_scene();

    auto sprites = _registry.group<const SpriteRenderer2DComponent>();

    for (auto&& [_, sprite] : sprites.each())
    {
        auto& texture = sprite.texture();
        ZTH_ASSERT(texture != nullptr);
        Renderer2D::submit(rect_in_pixel_coordinates_to_rect_in_ndc(sprite.rect, Renderer2D::viewport()), *texture,
                           sprite.color);
    }

    Renderer2D::end_scene();
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

auto Scene::load() -> void
{
    ZTH_INTERNAL_TRACE("Loading scene \"{}\"...", _name);
    on_load();
    ZTH_INTERNAL_TRACE("Scene \"{}\" loaded.", _name);
}

auto Scene::unload() -> void
{
    ZTH_INTERNAL_TRACE("Unloading scene \"{}\"...", _name);
    on_unload();
    _registry.clear();
    ZTH_INTERNAL_TRACE("Scene \"{}\" unloaded.", _name);
}

auto Scene::set_up_registry_listeners() -> void
{
    _registry.add_on_attach_listener<ScriptComponent, [](Registry& registry, EntityId entity_id) {
        EntityHandle entity{ entity_id, registry };
        auto& script = entity.get<ScriptComponent>();
        script.script().on_attach(entity);
    }>();

    _registry.add_on_detach_listener<ScriptComponent, [](Registry& registry, EntityId entity_id) {
        EntityHandle entity{ entity_id, registry };
        auto& script = entity.get<ScriptComponent>();
        script.script().on_detach(entity);
    }>();
}

auto SceneManager::init() -> Result<void, String>
{
    ZTH_INTERNAL_TRACE("Initializing scene manager...");
    _scene = make_unique<Scene>();
    _scene->load();
    ZTH_INTERNAL_TRACE("Scene manager initialized.");
    return {};
}

auto SceneManager::start_frame() -> void
{
    if (_queued_scene_factory)
    {
        ZTH_INTERNAL_TRACE("[SceneManager] Changing scenes.");

        _scene->unload();
        _scene.free();

        _scene = _queued_scene_factory();
        _scene->load();

        _queued_scene_factory = {};
    }

    _scene->start_frame();
}

auto SceneManager::dispatch_event(const Event& event) -> void
{
    _scene->dispatch_event(event);
}

auto SceneManager::fixed_update() -> void
{
    _scene->fixed_update();
}

auto SceneManager::update() -> void
{
    _scene->update();
}

auto SceneManager::render() -> void
{
    _scene->render();
}

auto SceneManager::on_render() -> void
{
    _scene->on_render();
}

auto SceneManager::shut_down() -> void
{
    ZTH_INTERNAL_TRACE("Shutting down scene manager...");

    _scene->unload();
    _scene.free();

    ZTH_INTERNAL_TRACE("Scene manager shut down.");
}

auto SceneManager::queue_scene(const std::function<UniquePtr<Scene>()>& factory) -> void
{
    _queued_scene_factory = factory;
}

auto SceneManager::queue_scene(std::function<UniquePtr<Scene>()>&& factory) -> void
{
    _queued_scene_factory = std::move(factory);
}

auto SceneManager::scene() -> Scene&
{
    return *_scene;
}

} // namespace zth
