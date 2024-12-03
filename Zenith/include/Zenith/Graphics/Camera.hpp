#pragma once

#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

namespace zth {

class Camera
{
public:
    virtual ~Camera() = default;

    [[nodiscard]] virtual auto view_projection() const -> const glm::mat4& = 0;
};

class PerspectiveCamera : public Camera
{
public:
    explicit PerspectiveCamera(glm::vec3 position, glm::vec3 front, float aspect_ratio,
                               float fov = glm::radians(45.0f));
    explicit PerspectiveCamera(glm::vec3 position, float yaw, float pitch, float aspect_ratio,
                               float fov = glm::radians(45.0f));

    auto update_view_projection() -> void;

    auto set_position(glm::vec3 position) -> void;
    auto set_aspect_ratio(float aspect_ratio) -> void;
    auto set_fov(float fov) -> void;

    auto set_yaw(float yaw) -> void;
    auto set_pitch(float pitch) -> void;
    auto set_yaw_and_pitch(float yaw, float pitch) -> void;

    [[nodiscard]] auto position() const { return _position; }

    [[nodiscard]] auto aspect_ratio() const { return _aspect_ratio; }
    [[nodiscard]] auto fov() const { return _fov; }

    [[nodiscard]] auto front() const { return _front; }
    [[nodiscard]] auto right() const { return _right; }
    [[nodiscard]] auto up() const { return _up; }

    [[nodiscard]] auto yaw() const { return _yaw; }
    [[nodiscard]] auto pitch() const { return _pitch; }

    [[nodiscard]] auto view_projection() const -> const glm::mat4& override { return _view_projection; }

private:
    glm::vec3 _position;

    float _aspect_ratio;
    float _fov;

    // these three vectors represent the camera's coordinate system
    glm::vec3 _front;
    glm::vec3 _right;
    glm::vec3 _up;

    float _yaw;
    float _pitch;

    glm::mat4 _view_projection;

private:
    auto update_front() -> void;
    auto update_right() -> void;
    auto update_up() -> void;

    auto update_direction_vectors() -> void;
};

} // namespace zth
