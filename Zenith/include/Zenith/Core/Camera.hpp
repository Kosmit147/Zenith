#pragma once

#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

namespace zth {

class Camera
{
public:
    glm::vec3 position;

    float aspect_ratio;
    float fov;

public:
    explicit Camera(glm::vec3 position, glm::vec3 front, float aspect_ratio, float fov = glm::radians(45.0f));
    explicit Camera(glm::vec3 position, float yaw, float pitch, float aspect_ratio, float fov = glm::radians(45.0f));

    [[nodiscard]] auto view() const -> glm::mat4;
    [[nodiscard]] auto projection() const -> glm::mat4;

    auto set_yaw(float yaw) -> void;
    auto set_pitch(float pitch) -> void;
    auto set_yaw_and_pitch(float yaw, float pitch) -> void;

    [[nodiscard]] auto front() const { return _front; }
    [[nodiscard]] auto right() const { return _right; }
    [[nodiscard]] auto up() const { return _up; }

    [[nodiscard]] auto yaw() const { return _yaw; }
    [[nodiscard]] auto pitch() const { return _pitch; }

private:
    // these three vectors represent the camera's coordinate system
    glm::vec3 _front;
    glm::vec3 _right;
    glm::vec3 _up;

    float _yaw;
    float _pitch;

private:
    auto update_front() -> void;
    auto update_right() -> void;
    auto update_up() -> void;

    auto update_direction_vectors() -> void;
};

} // namespace zth
