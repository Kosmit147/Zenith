#include "zenith/physics/layer.hpp"

#include "zenith/core/assert.hpp"

namespace zth::physics {

auto ObjectLayerPairFilter::ShouldCollide(JPH::ObjectLayer first, JPH::ObjectLayer second) const -> bool
{
    // Only dynamic objects collide.
    return first == dynamic_objects_layer || second == dynamic_objects_layer;
}

auto BroadPhaseLayerInterface::GetNumBroadPhaseLayers() const -> JPH::uint
{
    return broad_phase_layer_count;
}

auto BroadPhaseLayerInterface::GetBroadPhaseLayer(JPH::ObjectLayer object_layer) const -> JPH::BroadPhaseLayer
{
    ZTH_ASSERT(object_layer < object_layer_count);

    switch (object_layer)
    {
    case static_objects_layer:
        return static_objects_broad_phase_layer;
    case dynamic_objects_layer:
        return dynamic_objects_broad_phase_layer;
    }

    ZTH_ASSERT(false);
    std::unreachable();
}

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)

auto BroadPhaseLayerInterface::GetBroadPhaseLayerName(JPH::BroadPhaseLayer layer) const -> const char*
{
    ZTH_ASSERT(static_cast<JPH::BroadPhaseLayer::Type>(layer) < broad_phase_layer_count);

    if (layer == static_objects_broad_phase_layer)
        return "Static Objects Broad Phase Layer";
    else if (layer == dynamic_objects_broad_phase_layer)
        return "Dynamic Objects Broad Phase Layer";

    ZTH_ASSERT(false);
    std::unreachable();
}

#endif

auto ObjectVsBroadPhaseLayerFilter::ShouldCollide(JPH::ObjectLayer object_layer,
                                                  JPH::BroadPhaseLayer broad_phase_layer) const -> bool
{
    // Only dynamic objects collide.
    return object_layer == dynamic_objects_layer || broad_phase_layer == dynamic_objects_broad_phase_layer;
}

} // namespace zth::physics
