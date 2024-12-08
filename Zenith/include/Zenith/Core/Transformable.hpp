#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace zth {

class Transformable
{
public:
    explicit Transformable() = default;
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
    explicit Transformable3D(const glm::mat4& transform);
    ~Transformable3D() override = default;

    auto translate(glm::vec3 translation) -> Transformable3D&;
    auto rotate(float angle, glm::vec3 axis) -> Transformable3D&;
    auto scale(glm::vec3 factor) -> Transformable3D&;

    auto set_transform(const glm::mat4& transform) -> void;
    [[nodiscard]] auto transform() const -> auto& { return _transform; }

    [[nodiscard]] auto translation() const { return _translation; }
    [[nodiscard]] auto rotation() const { return _rotation; }
    [[nodiscard]] auto scale() const { return _scale; }

private:
    glm::vec3 _translation{ 0.0f };
    glm::quat _rotation{ glm::identity<glm::quat>() };
    glm::vec3 _scale{ 1.0f };

    glm::mat4 _transform{ 1.0f };

private:
    auto update_transform() -> void;
};

} // namespace zth