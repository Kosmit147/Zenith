#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

#include <memory>

#include "zenith/log/format.hpp"
#include "zenith/math/quaternion.hpp"
#include "zenith/renderer/fwd.hpp"
#include "zenith/renderer/light.hpp"
#include "zenith/script/script.hpp"
#include "zenith/stl/string.hpp"

namespace zth {

// --------------------------- TagComponent ---------------------------

// TagComponent is integral for every entity.
struct TagComponent
{
    String tag;

    [[nodiscard]] static auto display_label() -> const char*;
};

// --------------------------- TransformComponent ---------------------------

// TransformComponent is integral for every entity.
class TransformComponent
{
public:
    explicit TransformComponent() = default;
    explicit TransformComponent(glm::vec3 translation);
    explicit TransformComponent(glm::vec3 translation, glm::vec3 direction); // direction must be normalized.
    explicit TransformComponent(glm::vec3 translation, glm::quat rotation);
    explicit TransformComponent(glm::vec3 translation, math::AngleAxis rotation);
    explicit TransformComponent(glm::vec3 translation, math::EulerAngles rotation);
    explicit TransformComponent(glm::vec3 translation, glm::vec3 direction,
                                glm::vec3 scale); // direction must be normalized.
    explicit TransformComponent(glm::vec3 translation, glm::quat rotation, glm::vec3 scale);
    explicit TransformComponent(glm::vec3 translation, math::AngleAxis rotation, glm::vec3 scale);
    explicit TransformComponent(glm::vec3 translation, math::EulerAngles rotation, glm::vec3 scale);
    explicit TransformComponent(glm::vec3 translation, glm::vec3 direction,
                                float scale); // direction must be normalized.
    explicit TransformComponent(glm::vec3 translation, glm::quat rotation, float scale);
    explicit TransformComponent(glm::vec3 translation, math::AngleAxis rotation, float scale);
    explicit TransformComponent(glm::vec3 translation, math::EulerAngles rotation, float scale);
    explicit TransformComponent(const glm::mat4& transform);

    auto translate(glm::vec3 translation) -> TransformComponent&;

    auto rotate(float angle, glm::vec3 axis) -> TransformComponent&; // axis must be normalized.
    auto rotate(math::AngleAxis rotation) -> TransformComponent&;    // axis must be normalized.
    auto rotate(glm::quat rotation) -> TransformComponent&;          // rotation must be normalized.
    auto rotate(math::EulerAngles rotation) -> TransformComponent&;  // rotation must be normalized.

    auto scale(float factor) -> TransformComponent&;
    auto scale(glm::vec3 factor) -> TransformComponent&;

    auto set_translation(glm::vec3 translation) -> TransformComponent&;

    auto set_rotation(float angle, glm::vec3 axis) -> TransformComponent&; // axis must be normalized.
    auto set_rotation(math::AngleAxis rotation) -> TransformComponent&;    // axis must be normalized.
    auto set_rotation(glm::quat rotation) -> TransformComponent&;          // rotation must be normalized.
    auto set_rotation(math::EulerAngles rotation) -> TransformComponent&;

    auto set_direction(glm::vec3 direction) -> TransformComponent&; // direction must be normalized.
    auto set_direction(glm::vec3 direction, glm::vec3 up)
        -> TransformComponent&; // direction and up must be normalized.

    auto set_scale(float scale) -> TransformComponent&;
    auto set_scale(glm::vec3 scale) -> TransformComponent&;

    auto set_transform(const glm::mat4& transform) -> TransformComponent&;

    [[nodiscard]] auto translation() const { return _translation; }
    [[nodiscard]] auto rotation() const { return _rotation; }
    [[nodiscard]] auto euler_angles() const -> math::EulerAngles;
    [[nodiscard]] auto direction() const -> glm::vec3;
    [[nodiscard]] auto scale() const { return _scale; }

    [[nodiscard]] auto forward() const -> glm::vec3;
    [[nodiscard]] auto right() const -> glm::vec3;
    [[nodiscard]] auto up() const -> glm::vec3;

    [[nodiscard]] auto transform() const -> auto& { return _transform; }

    [[nodiscard]] static auto display_label() -> const char*;

private:
    // @speed: Maybe we should not store the matrix in here and instead always recreate it with the call to transform().
    // That would be a lot better for cache efficiency when submitting the transforms to the renderer.
    glm::mat4 _transform{ 1.0f };

    glm::vec3 _translation{ 0.0f };
    glm::quat _rotation{ glm::identity<glm::quat>() };
    glm::vec3 _scale{ 1.0f };

private:
    auto update_transform() -> void;
};

// --------------------------- ScriptComponent ---------------------------

class ScriptComponent
{
public:
    explicit ScriptComponent(std::unique_ptr<Script>&& script);

    [[nodiscard]] auto script() -> Script&;
    [[nodiscard]] auto script() const -> const Script&;

    [[nodiscard]] static auto display_label() -> const char*;

private:
    std::unique_ptr<Script> _script; // script cannot be null.
};

// --------------------------- MeshComponent ---------------------------

struct MeshComponent
{
    std::shared_ptr<const Mesh> mesh; // mesh cannot be null.

    [[nodiscard]] static auto display_label() -> const char*;
};

// --------------------------- MaterialComponent ---------------------------

struct MaterialComponent
{
    std::shared_ptr<const Material> material; // material cannot be null.

    [[nodiscard]] static auto display_label() -> const char*;
};

// --------------------------- CameraComponent ---------------------------

// @todo: Orthographic camera.

struct CameraComponent
{
    float aspect_ratio = 1.0f;
    float fov = glm::radians(45.0f);
    float near = 0.1f;
    float far = 100.0f;

    [[nodiscard]] auto view(const TransformComponent& transform) const -> glm::mat4;
    [[nodiscard]] auto projection() const -> glm::mat4;
    [[nodiscard]] auto view_projection(const TransformComponent& transform) const -> glm::mat4;

    [[nodiscard]] static auto display_label() -> const char*;
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

    [[nodiscard]] static auto display_label() -> const char*;

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

// --------------------------- DeletionMarkerComponent ---------------------------

struct DeletionMarkerComponent {};

} // namespace zth

ZTH_DECLARE_FORMATTER(zth::TagComponent);
ZTH_DECLARE_FORMATTER(zth::TransformComponent);
ZTH_DECLARE_FORMATTER(zth::ScriptComponent);
ZTH_DECLARE_FORMATTER(zth::MeshComponent);
ZTH_DECLARE_FORMATTER(zth::MaterialComponent);
ZTH_DECLARE_FORMATTER(zth::CameraComponent);
ZTH_DECLARE_FORMATTER(zth::LightComponent);
