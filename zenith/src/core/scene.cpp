#include "zenith/core/scene.hpp"

#include "zenith/core/assert.hpp"
#include "zenith/core/profiler.hpp"
#include "zenith/ecs/components.hpp"
#include "zenith/log/logger.hpp"
#include "zenith/physics/physics.hpp"
#include "zenith/renderer/coordinate_space.hpp"
#include "zenith/renderer/renderer.hpp"

namespace zth {

using namespace JPH::literals;

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

    // @todo: Should character controller be updated before other bodies or after?
    auto character_controllers = _registry.view<CharacterControllerComponent>();

    for (auto&& [entity_id, controller] : character_controllers.each())
    {
        Physics::update_character(controller.character);
        auto position = Physics::fetch_character_position(controller.character);
        auto& transform = _registry.get<TransformComponent>(entity_id);
        transform.set_translation(position);
    }

    auto dynamic_boxes = _registry.view<RigidBodyComponent, BoxColliderComponent>();

    for (auto&& [entity_id, rb, box_collider] : dynamic_boxes.each())
    {
        auto& transform = _registry.get<TransformComponent>(entity_id);
        auto [position, rotation] = Physics::fetch_body_transform(box_collider.body_id);
        transform.set_translation(position);
        transform.set_rotation(rotation);
    }

    auto dynamic_spheres = _registry.view<RigidBodyComponent, SphereColliderComponent>();

    for (auto&& [entity_id, rb, sphere_collider] : dynamic_spheres.each())
    {
        auto& transform = _registry.get<TransformComponent>(entity_id);
        auto [position, rotation] = Physics::fetch_body_transform(sphere_collider.body_id);
        transform.set_translation(position);
        transform.set_rotation(rotation);
    }

    auto dynamic_capsules = _registry.view<RigidBodyComponent, CapsuleColliderComponent>();

    for (auto&& [entity_id, rb, capsule_collider] : dynamic_capsules.each())
    {
        auto& transform = _registry.get<TransformComponent>(entity_id);
        auto [position, rotation] = Physics::fetch_body_transform(capsule_collider.body_id);
        transform.set_translation(position);
        transform.set_rotation(rotation);
    }

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
    ZTH_INTERNAL_TRACE("[Scene] Loading scene \"{}\"...", _name);
    on_load();
    ZTH_INTERNAL_TRACE("[Scene] Scene \"{}\" loaded.", _name);
}

auto Scene::unload() -> void
{
    ZTH_INTERNAL_TRACE("[Scene] Unloading scene \"{}\"...", _name);
    on_unload();
    _registry.clear();
    ZTH_INTERNAL_TRACE("[Scene] Scene \"{}\" unloaded.", _name);
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

    _registry.add_on_attach_listener<BoxColliderComponent, [](Registry& registry, EntityId entity_id) {
        EntityHandle entity{ entity_id, registry };

        const auto& transform = entity.transform();
        auto translation = transform.translation();
        auto rotation = transform.rotation();
        auto scale = transform.scale();

        auto motion_type = entity.any_of<RigidBodyComponent>() ? JPH::EMotionType::Dynamic : JPH::EMotionType::Static;
        JPH::RVec3 shape_half_extent = JPH::RVec3{ scale.x, scale.y, scale.z } / 2.0_r;
        JPH::RVec3 shape_position{ translation.x, translation.y, translation.z };
        JPH::Quat shape_rotation{ rotation.x, rotation.y, rotation.z, rotation.w };

        auto body_id = Physics::add_box(entity_id, motion_type, shape_half_extent, shape_position, shape_rotation);
        entity.patch<BoxColliderComponent>([body_id](auto& box_collider) { box_collider.body_id = body_id; });
    }>();

    _registry.add_on_detach_listener<BoxColliderComponent, [](Registry& registry, EntityId entity_id) {
        EntityHandle entity{ entity_id, registry };
        auto& [id] = entity.get<const BoxColliderComponent>();
        Physics::remove_body(id);
    }>();

    _registry.add_on_attach_listener<SphereColliderComponent, [](Registry& registry, EntityId entity_id) {
        EntityHandle entity{ entity_id, registry };

        const auto& transform = entity.transform();
        auto translation = transform.translation();
        auto rotation = transform.rotation();
        auto scale = transform.scale();

        auto motion_type = entity.any_of<RigidBodyComponent>() ? JPH::EMotionType::Dynamic : JPH::EMotionType::Static;
        JPH::Real shape_radius = math::max_component(scale) / 2.0_r;
        JPH::RVec3 shape_position{ translation.x, translation.y, translation.z };
        JPH::Quat shape_rotation{ rotation.x, rotation.y, rotation.z, rotation.w };

        auto body_id = Physics::add_sphere(entity_id, motion_type, shape_radius, shape_position, shape_rotation);
        entity.patch<SphereColliderComponent>([body_id](auto& sphere_collider) { sphere_collider.body_id = body_id; });
    }>();

    _registry.add_on_detach_listener<SphereColliderComponent, [](Registry& registry, EntityId entity_id) {
        EntityHandle entity{ entity_id, registry };
        auto& [id] = entity.get<const SphereColliderComponent>();
        Physics::remove_body(id);
    }>();

    _registry.add_on_attach_listener<CapsuleColliderComponent, [](Registry& registry, EntityId entity_id) {
        EntityHandle entity{ entity_id, registry };

        const auto& transform = entity.transform();
        auto translation = transform.translation();
        auto rotation = transform.rotation();
        auto scale = transform.scale();

        auto motion_type = entity.any_of<RigidBodyComponent>() ? JPH::EMotionType::Dynamic : JPH::EMotionType::Static;
        JPH::Real shape_cylinder_height{ 0.8f * scale.y };
        JPH::Real shape_radius = 0.2f * scale.y;
        JPH::RVec3 shape_position{ translation.x, translation.y, translation.z };
        JPH::Quat shape_rotation{ rotation.x, rotation.y, rotation.z, rotation.w };

        auto body_id = Physics::add_capsule(entity_id, motion_type, shape_cylinder_height, shape_radius, shape_position,
                                            shape_rotation);
        entity.patch<CapsuleColliderComponent>(
            [body_id](auto& capsule_collider) { capsule_collider.body_id = body_id; });
    }>();

    _registry.add_on_detach_listener<CapsuleColliderComponent, [](Registry& registry, EntityId entity_id) {
        EntityHandle entity{ entity_id, registry };
        auto& [id] = entity.get<const CapsuleColliderComponent>();
        Physics::remove_body(id);
    }>();

    _registry.add_on_attach_listener<MeshColliderComponent, [](Registry& registry, EntityId entity_id) {
        EntityHandle entity{ entity_id, registry };

        const auto& transform = entity.transform();
        auto translation = transform.translation();
        auto rotation = transform.rotation();

        auto& [_, mesh] = entity.get<const MeshColliderComponent>();

        auto motion_type = entity.any_of<RigidBodyComponent>() ? JPH::EMotionType::Dynamic : JPH::EMotionType::Static;
        JPH::RVec3 shape_position{ translation.x, translation.y, translation.z };
        JPH::Quat shape_rotation{ rotation.x, rotation.y, rotation.z, rotation.w };

        auto body_id = Physics::add_mesh(entity_id, motion_type, mesh, shape_position, shape_rotation);
        entity.patch<MeshColliderComponent>([body_id](auto& mesh_collider) { mesh_collider.body_id = body_id; });
    }>();

    _registry.add_on_detach_listener<MeshColliderComponent, [](Registry& registry, EntityId entity_id) {
        EntityHandle entity{ entity_id, registry };
        auto& [id, mesh] = entity.get<const MeshColliderComponent>();
        Physics::remove_body(id);
    }>();

    _registry.add_on_attach_listener<RigidBodyComponent, [](Registry& registry, EntityId entity_id) {
        EntityHandle entity{ entity_id, registry };

        if (auto box_collider = entity.try_get<BoxColliderComponent>())
            Physics::make_body_dynamic(box_collider->get().body_id);

        if (auto sphere_collider = entity.try_get<SphereColliderComponent>())
            Physics::make_body_dynamic(sphere_collider->get().body_id);

        if (auto capsule_collider = entity.try_get<CapsuleColliderComponent>())
            Physics::make_body_dynamic(capsule_collider->get().body_id);

        if (auto mesh_collider = entity.try_get<MeshColliderComponent>())
            Physics::make_body_dynamic(mesh_collider->get().body_id);
    }>();

    _registry.add_on_detach_listener<RigidBodyComponent, [](Registry& registry, EntityId entity_id) {
        EntityHandle entity{ entity_id, registry };

        if (auto box_collider = entity.try_get<BoxColliderComponent>())
            Physics::make_body_static(box_collider->get().body_id);

        if (auto sphere_collider = entity.try_get<SphereColliderComponent>())
            Physics::make_body_static(sphere_collider->get().body_id);

        if (auto capsule_collider = entity.try_get<CapsuleColliderComponent>())
            Physics::make_body_static(capsule_collider->get().body_id);

        if (auto mesh_collider = entity.try_get<MeshColliderComponent>())
            Physics::make_body_static(mesh_collider->get().body_id);
    }>();

    _registry.add_on_attach_listener<CharacterControllerComponent, [](Registry& registry, EntityId entity_id) {
        EntityHandle entity{ entity_id, registry };

        const auto& transform = entity.transform();
        auto translation = transform.translation();
        auto rotation = transform.rotation();
        auto scale = transform.scale();

        JPH::Real character_capsule_cylinder_height{ 0.8f * scale.y };
        JPH::Real character_capsule_radius = 0.2f * scale.y;
        JPH::RVec3 character_position{ translation.x, translation.y, translation.z };
        JPH::Quat character_rotation{ rotation.x, rotation.y, rotation.z, rotation.w };

        auto character = Physics::add_character(entity_id, character_capsule_cylinder_height, character_capsule_radius,
                                                character_position, character_rotation);
        entity.patch<CharacterControllerComponent>(
            [character](auto& character_controller) { character_controller.character = character; });
    }>();

    // _registry.add_on_detach_listener<CharacterControllerComponent, [](Registry& registry, EntityId entity_id) {
    //     EntityHandle entity{ entity_id, registry };
    //     auto& [id] = entity.get<const CharacterControllerComponent>();
    //
    //     // I don't think we need to remove the character.
    //     // Physics::remove_body(id);
    // }>();
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
