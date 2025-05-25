#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>

#include "zenith/core/typedefs.hpp"

namespace zth::physics {

// @volatile: Implementation of ObjectLayerPairFilter, BroadPhaseLayerInterface and ObjectVsBroadPhaseLayerFilter
// depends on these values.
constexpr inline JPH::ObjectLayer static_objects_layer = 0;
constexpr inline JPH::ObjectLayer dynamic_objects_layer = 1;
constexpr inline usize object_layer_count = 2;

// @volatile: Implementation of BroadPhaseLayerInterface and ObjectVsBroadPhaseLayerFilter depends on these values.
constexpr inline JPH::BroadPhaseLayer static_objects_broad_phase_layer{ 0 };
constexpr inline JPH::BroadPhaseLayer dynamic_objects_broad_phase_layer{ 1 };
constexpr inline usize broad_phase_layer_count = 2;

// Determines if two object layers can collide.
class ObjectLayerPairFilter : public JPH::ObjectLayerPairFilter
{
public:
    [[nodiscard]] auto ShouldCollide(JPH::ObjectLayer first, JPH::ObjectLayer second) const -> bool override;
};

// Defines a mapping between an object layer and a broad phase layer.
class BroadPhaseLayerInterface : public JPH::BroadPhaseLayerInterface
{
public:
    [[nodiscard]] auto GetNumBroadPhaseLayers() const -> JPH::uint override;
    [[nodiscard]] auto GetBroadPhaseLayer(JPH::ObjectLayer object_layer) const -> JPH::BroadPhaseLayer override;

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
    [[nodiscard]] auto GetBroadPhaseLayerName(JPH::BroadPhaseLayer layer) const -> const char* override;
#endif
};

// Determines if an object layer can collide with a broad phase layer.
class ObjectVsBroadPhaseLayerFilter : public JPH::ObjectVsBroadPhaseLayerFilter
{
public:
    [[nodiscard]] auto ShouldCollide(JPH::ObjectLayer object_layer, JPH::BroadPhaseLayer broad_phase_layer) const
        -> bool override;
};

} // namespace zth::physics
