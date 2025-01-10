#pragma once

#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

#include "Zenith/Math/Quaternion.hpp"

namespace zth {

class PerspectiveCamera
{
public:
    explicit PerspectiveCamera(glm::vec3 translation, glm::vec3 direction, float aspect_ratio,
                               float fov = glm::radians(45.0f));
    explicit PerspectiveCamera(glm::vec3 translation, math::EulerAngles rotation, float aspect_ratio,
                               float fov = glm::radians(45.0f));

    auto set_aspect_ratio(float aspect_ratio) -> void;
    auto set_fov(float fov) -> void;

    auto translate(glm::vec3 translation) -> PerspectiveCamera&;
    auto rotate(float pitch, float yaw, float roll) -> PerspectiveCamera&;
    auto rotate(math::EulerAngles rotation) -> PerspectiveCamera&;

    auto set_translation(glm::vec3 translation) -> PerspectiveCamera&;
    auto set_rotation(float pitch, float yaw, float roll) -> PerspectiveCamera&;
    auto set_rotation(math::EulerAngles rotation) -> PerspectiveCamera&;
    auto set_translation_and_rotation(glm::vec3 translation, math::EulerAngles rotation) -> PerspectiveCamera&;

    [[nodiscard]] auto aspect_ratio() const { return _aspect_ratio; }
    [[nodiscard]] auto fov() const { return _fov; }

    [[nodiscard]] auto translation() const { return _translation; }

    [[nodiscard]] auto pitch() const { return _pitch; }
    [[nodiscard]] auto yaw() const { return _yaw; }
    [[nodiscard]] auto roll() const { return _roll; }
    [[nodiscard]] auto rotation() const -> math::EulerAngles;

    [[nodiscard]] auto forward() const { return _forward; }
    [[nodiscard]] auto right() const { return _right; }
    [[nodiscard]] auto up() const { return _up; }

    [[nodiscard]] auto view() const -> auto& { return _view; }
    [[nodiscard]] auto projection() const -> auto& { return _projection; }
    [[nodiscard]] auto view_projection() const -> auto& { return _view_projection; }

private:
    float _aspect_ratio;
    float _fov;

    glm::vec3 _translation;

    float _pitch;
    float _yaw;
    float _roll;

    // these three vectors represent the camera's coordinate system
    glm::vec3 _forward;
    glm::vec3 _right;
    glm::vec3 _up;

    glm::mat4 _view{ 1.0f };
    glm::mat4 _projection{ 1.0f };
    glm::mat4 _view_projection{ 1.0f };

private:
    auto update_direction_vectors() -> void;
    auto update_view_projection() -> void;
};

} // namespace zth
