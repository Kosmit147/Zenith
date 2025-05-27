#include "zenith/physics/physics.hpp"

#include <Jolt/Jolt.h>
#include <imgui.h>

#include <Jolt/Core/Factory.h>
#include <Jolt/Core/IssueReporting.h>
#include <Jolt/Core/Memory.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Collision/CastResult.h>
#include <Jolt/Physics/Collision/NarrowPhaseQuery.h>
#include <Jolt/Physics/Collision/RayCast.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
#include <Jolt/Physics/Collision/Shape/MeshShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/RegisterTypes.h>
#include <glm/ext/quaternion_float.hpp>
#include <glm/vec3.hpp>

#include <cstdarg>

#include "zenith/core/assert.hpp"
#include "zenith/log/logger.hpp"
#include "zenith/memory/managed.hpp"
#include "zenith/memory/memory.hpp"
#include "zenith/renderer/mesh.hpp"
#include "zenith/system/time.hpp"

// @todo: Do we need this?
JPH_SUPPRESS_WARNINGS

namespace zth {

using namespace memory::size_literals;
using namespace JPH::literals;

namespace {

UniquePtr<Physics> physics_instance;

auto jolt_trace_callback(const char* fmt, ...) -> void
{
    va_list list;
    va_start(list, fmt);
    char buffer[1024];
    auto ret = vsnprintf(buffer, sizeof(buffer), fmt, list);
    ZTH_ASSERT(ret >= 0);
    (void)ret;
    va_end(list);

    ZTH_INTERNAL_TRACE("[Physics] {}", buffer);
}

#if defined(JPH_ENABLE_ASSERTS)

auto jolt_assert_failed_callback(const char* expression, const char* message, const char* file, JPH::uint line) -> bool
{
    std::cerr << "[Jolt] " << file << '(' << line << ") `" << "`:\n" << message << ": (" << expression << ")\n";

    // Returning true causes Jolt to trigger a breakpoint.
    return true;
}

#endif

} // namespace

// This constructor exists only for the purpose of allowing make_unique to construct an instance of Physics.
Physics::Physics(Passkey, const PhysicsSpec& spec) : Physics(spec) {}

auto Physics::init(const PhysicsSpec& spec) -> Result<void, String>
{
    ZTH_INTERNAL_TRACE("Initializing physics...");

    JPH::RegisterDefaultAllocator();
    JPH::Trace = jolt_trace_callback;
    JPH_IF_ENABLE_ASSERTS(JPH::AssertFailed = jolt_assert_failed_callback);

    // @todo: Maybe use allocate_and_construct_using_allocator instead of using new here (or just a UniquePtr)?
    JPH::Factory::sInstance = new JPH::Factory();
    JPH::RegisterTypes();

    physics_instance = make_unique<Physics>(Passkey{}, spec);

    physics_instance->_physics_system.OptimizeBroadPhase();

    ZTH_INTERNAL_TRACE("Physics initialized.");
    return {};
}

auto Physics::fixed_update() -> void
{
    physics_instance->_physics_system.Update(Time::fixed_time_step<float>(), 1, &physics_instance->_temp_allocator,
                                             &physics_instance->_job_system);
}

auto Physics::shut_down() -> void
{
    ZTH_INTERNAL_TRACE("Shutting down physics...");

    JPH::UnregisterTypes();

    delete JPH::Factory::sInstance;
    JPH::Factory::sInstance = nullptr;

    ZTH_INTERNAL_TRACE("Physics shut down.");
}

auto Physics::add_box(EntityId entity_id, JPH::EMotionType motion_type, JPH::RVec3 half_extent, JPH::RVec3 position,
                      JPH::Quat rotation) -> physics::BodyId
{
    auto& body_interface = get_body_interface();

    auto layer = [=] {
        if (motion_type == JPH::EMotionType::Static)
            return physics::static_objects_layer;
        else if (motion_type == JPH::EMotionType::Dynamic)
            return physics::dynamic_objects_layer;

        ZTH_ASSERT(false);
        return physics::dynamic_objects_layer;
    }();

    JPH::BodyCreationSettings box_settings{ new JPH::BoxShape{ half_extent }, position, rotation, motion_type, layer };
    box_settings.mAllowDynamicOrKinematic = true;
    // We store the entity id in mUserData.
    box_settings.mUserData = static_cast<JPH::uint64>(std::to_underlying(entity_id));
    JPH::BodyID box_id = body_interface.CreateAndAddBody(box_settings, JPH::EActivation::Activate);
    return box_id;
}

auto Physics::add_sphere(EntityId entity_id, JPH::EMotionType motion_type, JPH::Real radius, JPH::RVec3 position,
                         JPH::Quat rotation) -> physics::BodyId
{
    auto& body_interface = get_body_interface();

    auto layer = [=] {
        if (motion_type == JPH::EMotionType::Static)
            return physics::static_objects_layer;
        else if (motion_type == JPH::EMotionType::Dynamic)
            return physics::dynamic_objects_layer;

        ZTH_ASSERT(false);
        return physics::dynamic_objects_layer;
    }();

    JPH::BodyCreationSettings sphere_settings{ new JPH::SphereShape{ radius }, position, rotation, motion_type, layer };
    sphere_settings.mAllowDynamicOrKinematic = true;
    // We store the entity id in mUserData.
    sphere_settings.mUserData = static_cast<JPH::uint64>(std::to_underlying(entity_id));
    auto sphere_id = body_interface.CreateAndAddBody(sphere_settings, JPH::EActivation::Activate);
    return sphere_id;
}

auto Physics::add_capsule(EntityId entity_id, JPH::EMotionType motion_type, JPH::Real half_cylinder_height,
                          JPH::Real radius, JPH::RVec3 position, JPH::Quat rotation) -> physics::BodyId
{
    auto& body_interface = get_body_interface();

    auto layer = [=] {
        if (motion_type == JPH::EMotionType::Static)
            return physics::static_objects_layer;
        else if (motion_type == JPH::EMotionType::Dynamic)
            return physics::dynamic_objects_layer;

        ZTH_ASSERT(false);
        return physics::dynamic_objects_layer;
    }();

    JPH::BodyCreationSettings capsule_settings{ new JPH::CapsuleShape{ half_cylinder_height, radius }, position,
                                                rotation, motion_type, layer };
    capsule_settings.mAllowDynamicOrKinematic = true;
    // We store the entity id in mUserData.
    capsule_settings.mUserData = static_cast<JPH::uint64>(std::to_underlying(entity_id));
    auto capsule_id = body_interface.CreateAndAddBody(capsule_settings, JPH::EActivation::Activate);
    return capsule_id;
}

auto Physics::add_mesh(EntityId entity_id, JPH::EMotionType motion_type, const std::shared_ptr<const Mesh>& mesh,
                       JPH::RVec3 position, JPH::Quat rotation) -> physics::BodyId
{
    auto& body_interface = get_body_interface();

    ZTH_ASSERT(motion_type == JPH::EMotionType::Static);

    auto layer = [=] {
        if (motion_type == JPH::EMotionType::Static)
            return physics::static_objects_layer;
        else if (motion_type == JPH::EMotionType::Dynamic)
            return physics::dynamic_objects_layer;

        ZTH_ASSERT(false);
        return physics::dynamic_objects_layer;
    }();

    JPH::TriangleList triangles{};

    if (auto quad_mesh = std::dynamic_pointer_cast<const QuadMesh<StandardVertex>>(mesh))
    {
        auto& vertices = quad_mesh->vertices();

        for (auto quad : vertices | std::views::chunk(vertices_per_quad))
        {
            JPH::Float3 v1{ quad[0].position.x, quad[0].position.y, quad[0].position.z };
            JPH::Float3 v2{ quad[1].position.x, quad[1].position.y, quad[1].position.z };
            JPH::Float3 v3{ quad[2].position.x, quad[2].position.y, quad[2].position.z };
            JPH::Float3 v4{ quad[3].position.x, quad[3].position.y, quad[3].position.z };

            JPH::Triangle triangle1{ v1, v2, v3 };
            JPH::Triangle triangle2{ v1, v3, v4 };

            triangles.push_back(triangle1);
            triangles.push_back(triangle2);
        }
    }
    else
    {
        ZTH_ASSERT(false);
    }

    JPH::Ref<JPH::MeshShapeSettings> mesh_shape_settings = new JPH::MeshShapeSettings(triangles);
    // mesh_shape_settings.SetEmbedded();

    JPH::ShapeSettings::ShapeResult mesh_shape_result = mesh_shape_settings->Create();
    ZTH_ASSERT(mesh_shape_result.IsValid());
    JPH::Ref<JPH::Shape> shape_ref = mesh_shape_result.Get();

    JPH::BodyCreationSettings mesh_body_settings{ shape_ref, position, rotation, motion_type, layer };
    // mesh_body_settings.mAllowDynamicOrKinematic = true;
    // We store the entity id in mUserData.
    mesh_body_settings.mUserData = static_cast<JPH::uint64>(std::to_underlying(entity_id));
    auto mesh_body_id = body_interface.CreateAndAddBody(mesh_body_settings, JPH::EActivation::Activate);
    return mesh_body_id;
}

auto Physics::add_character(EntityId entity_id, JPH::Real half_cylinder_height, JPH::Real radius, JPH::RVec3 position,
                            JPH::Quat rotation) -> JPH::Ref<JPH::CharacterVirtual>
{
    JPH::Ref character_settings = new JPH::CharacterVirtualSettings;
    character_settings->mShape = new JPH::CapsuleShape{ half_cylinder_height, radius };

    // We store the entity id in mUserData.
    auto user_data = static_cast<JPH::uint64>(std::to_underlying(entity_id));

    JPH::Ref character = new JPH::CharacterVirtual(character_settings, position, rotation, user_data,
                                                   &physics_instance->_physics_system);

    // character->SetCharacterVsCharacterCollision();

    return character;
}

auto Physics::make_body_static(physics::BodyId body_id) -> void
{
    auto& body_interface = get_body_interface();
    body_interface.SetObjectLayer(body_id, physics::static_objects_layer);
    body_interface.SetMotionType(body_id, JPH::EMotionType::Static, JPH::EActivation::DontActivate);
}

auto Physics::make_body_dynamic(physics::BodyId body_id) -> void
{
    auto& body_interface = get_body_interface();
    body_interface.SetObjectLayer(body_id, physics::dynamic_objects_layer);
    body_interface.SetMotionType(body_id, JPH::EMotionType::Dynamic, JPH::EActivation::Activate);
}

auto Physics::update_character(const JPH::Ref<JPH::CharacterVirtual>& character) -> void
{
    auto& physics_system = physics_instance->_physics_system;
    auto& temp_allocator = physics_instance->_temp_allocator;

    character->Update(Time::fixed_time_step<float>(), -character->GetUp() * physics_system.GetGravity(),
                      physics_system.GetDefaultBroadPhaseLayerFilter(physics::dynamic_objects_layer),
                      physics_system.GetDefaultLayerFilter(physics::dynamic_objects_layer), {}, {}, temp_allocator);
}

auto Physics::update_character_position(const JPH::Ref<JPH::CharacterVirtual>& character, glm::vec3 position) -> void
{
    character->SetPosition(JPH::RVec3{ position.x, position.y, position.z });
}

auto Physics::update_body_velocity(physics::BodyId body_id, glm::vec3 velocity) -> void
{
    auto& body_interface = get_body_interface();
    JPH::Vec3 new_velocity{ velocity.x, velocity.y, velocity.z };
    body_interface.SetLinearVelocity(body_id, new_velocity);
}

auto Physics::update_body_transform(physics::BodyId body_id, const physics::BodyTransform& transform) -> void
{
    auto& body_interface = get_body_interface();
    auto& [position, rotation] = transform;
    JPH::RVec3 new_position{ position.x, position.y, position.z };
    JPH::Quat new_rotation{ rotation.x, rotation.y, rotation.z, rotation.w };
    body_interface.SetPositionAndRotation(body_id, new_position, new_rotation, JPH::EActivation::Activate);
}

auto Physics::get_entity(physics::BodyId body_id) -> EntityId
{
    auto& body_interface = get_body_interface();
    return static_cast<EntityId>(body_interface.GetUserData(body_id));
}

auto Physics::fetch_body_transform(physics::BodyId body_id) -> physics::BodyTransform
{
    auto& body_interface = get_body_interface();

    JPH::RVec3 position;
    JPH::Quat rotation;
    body_interface.GetPositionAndRotation(body_id, position, rotation);

    return physics::BodyTransform{
        .position = glm::vec3{ position.GetX(), position.GetY(), position.GetZ() },
        .rotation = glm::quat{ rotation.GetW(), rotation.GetX(), rotation.GetY(), rotation.GetZ() },
    };
}

auto Physics::fetch_character_position(const JPH::Ref<JPH::CharacterVirtual>& character) -> glm::vec3
{
    auto position = character->GetPosition();
    return glm::vec3{ position.GetX(), position.GetY(), position.GetZ() };
}

auto Physics::remove_body(physics::BodyId body_id) -> void
{
    auto& body_interface = get_body_interface();
    body_interface.RemoveBody(body_id);
    body_interface.DestroyBody(body_id);
}

auto Physics::ray_cast(glm::vec3 origin, glm::vec3 ray) -> Optional<std::pair<physics::BodyId, glm::vec3>>
{
    auto& narrow_phase_query = physics_instance->_physics_system.GetNarrowPhaseQuery();

    JPH::Vec3 ray_origin{ origin.x, origin.y, origin.z };
    JPH::Vec3 ray_direction{ ray.x, ray.y, ray.z };
    JPH::RRayCast ray_cast{ ray_origin, ray_direction };
    JPH::RayCastResult result;

    if (narrow_phase_query.CastRay(ray_cast, result))
    {
        auto hit_point = origin + result.mFraction * ray;
        return std::make_pair(result.mBodyID, hit_point);
    }
    else
    {
        return nil;
    }
}

auto Physics::gravity() -> JPH::Vec3
{
    return physics_instance->_physics_system.GetGravity();
}

Physics::Physics(const PhysicsSpec& spec)
    : _temp_allocator{ 10_mb },
      _job_system{ spec.max_thread_pool_jobs, spec.max_thread_pool_barriers, static_cast<i32>(spec.threads) }
{
    _physics_system.Init(spec.max_bodies, spec.body_mutexes, spec.max_body_pairs, spec.max_contact_constraints,
                         _broad_phase_layer_interface, _object_vs_broad_phase_layer_filter, _object_layer_pair_filter);
}

auto Physics::get_body_interface() -> JPH::BodyInterface&
{
    return physics_instance->_physics_system.GetBodyInterface();
}

} // namespace zth
