#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyID.h>
#include <glm/ext/quaternion_float.hpp>
#include <glm/vec3.hpp>

namespace zth::physics {

// @todo: We should maybe make our own wrappers for these?
using BodyId = JPH::BodyID;
constexpr inline auto invalid_body_id = BodyId::cInvalidBodyID;

struct BodyTransform
{
    glm::vec3 position;
    glm::quat rotation;
};

} // namespace zth::physics
