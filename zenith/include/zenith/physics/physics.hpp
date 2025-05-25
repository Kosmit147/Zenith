#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Physics/Character/CharacterVirtual.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>

#include <memory>
#include <thread>

#include "zenith/ecs/ecs.hpp"
#include "zenith/physics/body.hpp"
#include "zenith/physics/layer.hpp"
#include "zenith/renderer/fwd.hpp"
#include "zenith/stl/string.hpp"
#include "zenith/util/optional.hpp"
#include "zenith/util/result.hpp"

namespace zth {

struct PhysicsSpec
{
    u32 max_thread_pool_jobs = JPH::cMaxPhysicsJobs;
    u32 max_thread_pool_barriers = JPH::cMaxPhysicsBarriers;
    u32 threads = std::thread::hardware_concurrency() - 1;

    // The maximum amount of rigid bodies that can be added to the system.
    u32 max_bodies = 65536;
    // How many mutexes to allocate to protect rigid bodies from concurrent access. Set it to 0 for default settings.
    u32 body_mutexes = 0;
    // Max amount of body pairs that can be queued at any given time.
    u32 max_body_pairs = 65536;
    // Maximum number of contact constraints. If more contacts are detected than this number then these contacts will be
    // ignored and bodies will start interpenetrating / falling through the world.
    u32 max_contact_constraints = 10240;
};

// Zenith uses Jolt as its backend for the physics system.
// Every object has to have a collider component in order to be registered by the physics system.
// Bodies can either be static or dynamic.
// If an entity has a rigid body component, it becomes a dynamic body and its transform is now fully controlled by the
// physics system.
// The entity id is stored in UserData public member of the Jolt Body.

namespace physics {

using CharacterId = JPH::CharacterID;
using CharacterController = JPH::CharacterVirtual;

} // namespace physics

class Physics
{
private:
    // clang-format off
    struct Passkey {}; // Allows constructing an instance of Physics through make_unique.
    // clang-format on

public:
    explicit Physics(Passkey, const PhysicsSpec& spec);

    [[nodiscard]] static auto init(const PhysicsSpec& spec) -> Result<void, String>;
    static auto fixed_update() -> void;
    static auto shut_down() -> void;

    // @todo: Use glm types in public interface.

    static auto add_box(EntityId entity_id, JPH::EMotionType motion_type, JPH::RVec3 half_extent, JPH::RVec3 position,
                        JPH::Quat rotation) -> physics::BodyId;
    static auto add_sphere(EntityId entity_id, JPH::EMotionType motion_type, JPH::Real radius,
                           JPH::RVec3 position = JPH::RVec3::sZero(), JPH::Quat rotation = JPH::Quat::sIdentity())
        -> physics::BodyId;
    static auto add_capsule(EntityId entity_id, JPH::EMotionType motion_type, JPH::Real half_cylinder_height,
                            JPH::Real radius, JPH::RVec3 position = JPH::RVec3::sZero(),
                            JPH::Quat rotation = JPH::Quat::sIdentity()) -> physics::BodyId;
    static auto add_mesh(EntityId entity_id, JPH::EMotionType motion_type, const std::shared_ptr<const Mesh>& mesh,
                         JPH::RVec3 position = JPH::RVec3::sZero(), JPH::Quat rotation = JPH::Quat::sIdentity())
        -> physics::BodyId;

    static auto add_character(EntityId entity_id, JPH::Real half_cylinder_height, JPH::Real radius,
                              JPH::RVec3 position = JPH::RVec3::sZero(), JPH::Quat rotation = JPH::Quat::sIdentity())
        -> JPH::Ref<JPH::CharacterVirtual>;

    static auto make_body_static(physics::BodyId body_id) -> void;
    static auto make_body_dynamic(physics::BodyId body_id) -> void;

    static auto update_character(const JPH::Ref<JPH::CharacterVirtual>& character) -> void;
    static auto update_body_velocity(physics::BodyId body_id, glm::vec3 velocity) -> void;
    static auto update_body_transform(physics::BodyId body_id, const physics::BodyTransform& transform) -> void;

    [[nodiscard]] static auto get_entity(physics::BodyId body_id) -> EntityId;
    [[nodiscard]] static auto fetch_body_transform(physics::BodyId body_id) -> physics::BodyTransform;
    [[nodiscard]] static auto fetch_character_position(const JPH::Ref<JPH::CharacterVirtual>& character) -> glm::vec3;

    static auto remove_body(physics::BodyId body_id) -> void;

    [[nodiscard]] static auto ray_cast(glm::vec3 origin, glm::vec3 ray)
        -> Optional<std::pair<physics::BodyId, glm::vec3>>;

    [[nodiscard]] static auto gravity() -> JPH::Vec3;

private:
    // @todo: Use our own temporary allocator.
    JPH::TempAllocatorImpl _temp_allocator;
    JPH::JobSystemThreadPool _job_system;

    physics::ObjectLayerPairFilter _object_layer_pair_filter;
    physics::BroadPhaseLayerInterface _broad_phase_layer_interface;
    physics::ObjectVsBroadPhaseLayerFilter _object_vs_broad_phase_layer_filter;

    JPH::PhysicsSystem _physics_system;

private:
    explicit Physics(const PhysicsSpec& spec);

    [[nodiscard]] static auto get_body_interface() -> JPH::BodyInterface&;
};

} // namespace zth
