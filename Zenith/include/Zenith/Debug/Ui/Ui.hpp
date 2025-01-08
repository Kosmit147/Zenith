#pragma once

#include <glm/vec3.hpp>

#include "Zenith/Core/fwd.hpp"
#include "Zenith/Graphics/fwd.hpp"
#include "Zenith/Platform/Key.hpp"
#include "Zenith/Utility/Macros.hpp"

namespace zth {

struct KeyPressedEvent;

namespace debug {

class DebugToolsUi
{
public:
    Key toggle_wireframe_mode_key = Key::F1;

public:
    explicit DebugToolsUi() = default;
    ZTH_NO_COPY_NO_MOVE(DebugToolsUi)
    ~DebugToolsUi() = default;

    auto on_key_pressed_event(const KeyPressedEvent& event) -> void;
    auto on_update() -> void;

private:
    bool _wireframe_mode_enabled = false;
};

class TransformUi
{
public:
    explicit TransformUi(Transformable3D& transformable);
    ZTH_NO_COPY_NO_MOVE(TransformUi)
    ~TransformUi() = default;

    auto on_update() -> void;

private:
    Transformable3D& _transformable;

    glm::vec3 _translation = glm::vec3{ 0.0f };
    float _rotation_angle = 0.0f;
    glm::vec3 _rotation_axis = glm::vec3{ 0.0f, 1.0f, 0.0f };
    glm::vec3 _scale = glm::vec3{ 1.0f };
    bool _uniform_scale = true;
};

class DirectionalLightUi
{
public:
    explicit DirectionalLightUi(DirectionalLight& light);
    ZTH_NO_COPY_NO_MOVE(DirectionalLightUi)
    ~DirectionalLightUi() = default;

    auto on_update() -> void;

private:
    DirectionalLight& _light;
    glm::vec3 _light_direction;
};

class PointLightUi
{
public:
    explicit PointLightUi(PointLight& light);
    ZTH_NO_COPY_NO_MOVE(PointLightUi)
    ~PointLightUi() = default;

    auto on_update() -> void;

private:
    PointLight& _light;
    glm::vec3 _light_position;
};

} // namespace debug

} // namespace zth
