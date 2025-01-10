#pragma once

#include <imgui.h>
#include <ImGuizmo.h>
#include <glm/vec3.hpp>

#include "Zenith/Core/fwd.hpp"
#include "Zenith/Graphics/fwd.hpp"
#include "Zenith/Math/Quaternion.hpp"
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

    glm::vec3 _translation{ 0.0f };
    math::Rotation _rotation{};
    glm::vec3 _scale{ 1.0f };
    bool _uniform_scale = true;
};

class TransformGizmo
{
public:
    Key toggle_key = Key::LeftControl;
    Key switch_to_translate_mode_key = Key::Q;
    Key switch_to_rotate_mode_key = Key::E;
    Key switch_to_scale_mode_key = Key::R;

    bool enabled = false;

public:
    explicit TransformGizmo(Transformable3D& transformable, const PerspectiveCamera& camera);
    ZTH_NO_COPY_NO_MOVE(TransformGizmo)
    ~TransformGizmo() = default;

    auto on_update() -> void;
    auto on_key_pressed_event(const KeyPressedEvent& event) -> void;

private:
    Transformable3D& _transformable;
    const PerspectiveCamera& _camera;

    ImGuizmo::OPERATION _current_gizmo_operation = ImGuizmo::TRANSLATE;
    ImGuizmo::MODE _current_gizmo_mode = ImGuizmo::WORLD;
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
};

} // namespace debug

} // namespace zth
