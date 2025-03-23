#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <memory>

#include "zenith/log/format.hpp"
#include "zenith/math/quaternion.hpp"
#include "zenith/renderer/fwd.hpp"
#include "zenith/renderer/light.hpp"
#include "zenith/script/script.hpp"
#include "zenith/stl/string.hpp"
#include "zenith/util/macros.hpp"

namespace zth {

// --------------------------- TagComponent ---------------------------

// TagComponent is mandatory for every entity.
struct TagComponent
{
    String tag;
};

// --------------------------- TransformComponent ---------------------------

// TransformComponent is mandatory for every entity.
class TransformComponent
{
public:
    explicit TransformComponent() = default;
    explicit TransformComponent(glm::vec3 translation);
    explicit TransformComponent(glm::vec3 translation, glm::vec3 direction); // direction must be normalized.
    explicit TransformComponent(glm::vec3 translation, glm::quat rotation);
    explicit TransformComponent(glm::vec3 translation, math::Rotation rotation);
    explicit TransformComponent(glm::vec3 translation, math::EulerAngles rotation);
    explicit TransformComponent(glm::vec3 translation, glm::vec3 direction,
                                glm::vec3 scale); // direction must be normalized.
    explicit TransformComponent(glm::vec3 translation, glm::quat rotation, glm::vec3 scale);
    explicit TransformComponent(glm::vec3 translation, math::Rotation rotation, glm::vec3 scale);
    explicit TransformComponent(glm::vec3 translation, math::EulerAngles rotation, glm::vec3 scale);
    explicit TransformComponent(glm::vec3 translation, glm::vec3 direction,
                                float scale); // direction must be normalized.
    explicit TransformComponent(glm::vec3 translation, glm::quat rotation, float scale);
    explicit TransformComponent(glm::vec3 translation, math::Rotation rotation, float scale);
    explicit TransformComponent(glm::vec3 translation, math::EulerAngles rotation, float scale);
    explicit TransformComponent(const glm::mat4& transform);

    ZTH_DEFAULT_COPY_DEFAULT_MOVE(TransformComponent)

    ~TransformComponent() = default;

    auto translate(glm::vec3 translation) -> TransformComponent&;

    auto rotate(float angle, glm::vec3 axis) -> TransformComponent&; // axis must be normalized.
    auto rotate(math::Rotation rotation) -> TransformComponent&;     // axis must be normalized.
    auto rotate(glm::quat rotation) -> TransformComponent&;          // rotation must be normalized.
    auto rotate(math::EulerAngles rotation) -> TransformComponent&;  // rotation must be normalized.

    auto scale(float factor) -> TransformComponent&;
    auto scale(glm::vec3 factor) -> TransformComponent&;

    auto set_translation(glm::vec3 translation) -> TransformComponent&;

    auto set_rotation(float angle, glm::vec3 axis) -> TransformComponent&; // axis must be normalized.
    auto set_rotation(math::Rotation rotation) -> TransformComponent&;     // axis must be normalized.
    auto set_rotation(glm::quat rotation) -> TransformComponent&;          // rotation must be normalized.
    auto set_rotation(math::EulerAngles rotation) -> TransformComponent&;

    auto set_direction(glm::vec3 direction) -> TransformComponent&; // direction must be normalized.

    auto set_scale(float scale) -> TransformComponent&;
    auto set_scale(glm::vec3 scale) -> TransformComponent&;

    auto set_transform(const glm::mat4& transform) -> TransformComponent&;

    [[nodiscard]] auto translation() const { return _translation; }
    [[nodiscard]] auto rotation() const { return _rotation; }
    [[nodiscard]] auto direction() const -> glm::vec3;
    [[nodiscard]] auto scale() const { return _scale; }

    [[nodiscard]] auto transform() const -> auto& { return _transform; }

private:
    glm::mat4 _transform{ 1.0f };

    glm::vec3 _translation{ 0.0f };
    glm::quat _rotation{ glm::identity<glm::quat>() };
    glm::vec3 _scale{ 1.0f };

private:
    auto update_transform() -> void;
};

// --------------------------- ScriptComponent ---------------------------

struct ScriptComponent
{
    std::unique_ptr<Script> script; // script should never be null.
};

// --------------------------- MeshComponent ---------------------------

struct MeshComponent
{
    const Mesh* mesh;
};

// --------------------------- MaterialComponent ---------------------------

struct MaterialComponent
{
    const Material* material;
};

// --------------------------- CameraComponent ---------------------------

struct CameraComponent
{
    // @todo
};

// --------------------------- LightComponent ---------------------------

class LightComponent
{
public:
    LightComponent();
    LightComponent(const DirectionalLight& light);
    LightComponent(const PointLight& light);
    LightComponent(const SpotLight& light);
    LightComponent(const AmbientLight& light);

    auto set_light(LightType type) -> void;
    auto set_light(const DirectionalLight& light) -> void;
    auto set_light(const PointLight& light) -> void;
    auto set_light(const SpotLight& light) -> void;
    auto set_light(const AmbientLight& light) -> void;

    [[nodiscard]] auto type() const { return _type; }

    [[nodiscard]] auto directional_light() -> DirectionalLight&;
    [[nodiscard]] auto directional_light() const -> const DirectionalLight&;
    [[nodiscard]] auto point_light() -> PointLight&;
    [[nodiscard]] auto point_light() const -> const PointLight&;
    [[nodiscard]] auto spot_light() -> SpotLight&;
    [[nodiscard]] auto spot_light() const -> const SpotLight&;
    [[nodiscard]] auto ambient_light() -> AmbientLight&;
    [[nodiscard]] auto ambient_light() const -> const AmbientLight&;

    [[nodiscard]] auto to_directional_light_render_data(const TransformComponent& transform) const
        -> DirectionalLightRenderData;
    [[nodiscard]] auto to_point_light_render_data(const TransformComponent& transform) const -> PointLightRenderData;
    [[nodiscard]] auto to_spot_light_render_data(const TransformComponent& transform) const -> SpotLightRenderData;
    [[nodiscard]] auto to_ambient_light_render_data() const -> AmbientLightRenderData;

private:
    LightType _type;

    union
    {
        DirectionalLight _directional_light;
        PointLight _point_light;
        SpotLight _spot_light;
        AmbientLight _ambient_light;
    };
};

} // namespace zth

ZTH_DECLARE_FORMATTER(zth::TagComponent);
ZTH_DECLARE_FORMATTER(zth::TransformComponent);
ZTH_DECLARE_FORMATTER(zth::MeshComponent);
ZTH_DECLARE_FORMATTER(zth::MaterialComponent);
ZTH_DECLARE_FORMATTER(zth::CameraComponent);
ZTH_DECLARE_FORMATTER(zth::LightComponent);
