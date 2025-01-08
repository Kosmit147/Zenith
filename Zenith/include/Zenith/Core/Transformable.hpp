#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "Zenith/Utility/Macros.hpp"

namespace zth {

class Transformable
{
public:
    explicit Transformable() = default;
    ZTH_DEFAULT_COPY_DEFAULT_MOVE(Transformable)
    virtual ~Transformable() = default;
};

class Transformable2D : public Transformable
{
    // TODO
};

class Transformable3D : public Transformable
{
public:
    explicit Transformable3D() = default;
    explicit Transformable3D(glm::vec3 translation);
    explicit Transformable3D(const glm::mat4& transform);
    ZTH_DEFAULT_COPY_DEFAULT_MOVE(Transformable3D)
    ~Transformable3D() override = default;

    auto translate(glm::vec3 translation) -> Transformable3D&;
    auto rotate(float angle, glm::vec3 axis) -> Transformable3D&; // axis should be normalized
    auto scale(float factor) -> Transformable3D&;
    auto scale(glm::vec3 factor) -> Transformable3D&;

    auto set_translation(glm::vec3 translation) -> Transformable3D&;
    auto set_rotation(float angle, glm::vec3 axis) -> Transformable3D&; // axis should be normalized
    auto set_rotation(glm::quat rotation) -> Transformable3D&;
    auto set_direction(glm::vec3 direction) -> Transformable3D&; // direction should be normalized
    auto set_scale(float scale) -> Transformable3D&;
    auto set_scale(glm::vec3 scale) -> Transformable3D&;

    auto set_transform(const glm::mat4& transform) -> Transformable3D&;
    [[nodiscard]] auto transform() const -> auto& { return _transform; }

    [[nodiscard]] auto translation() const { return _translation; }
    [[nodiscard]] auto rotation() const { return _rotation; }
    [[nodiscard]] auto direction() const -> glm::vec3;
    [[nodiscard]] auto scale() const { return _scale; }

private:
    glm::vec3 _translation = glm::vec3{ 0.0f };
    glm::quat _rotation = glm::identity<glm::quat>();
    glm::vec3 _scale = glm::vec3{ 1.0f };

    glm::mat4 _transform = glm::mat4{ 1.0f };

private:
    auto update_transform() -> void;
};

} // namespace zth
