#pragma once

#include <imgui.h>
#include <ImGuizmo.h>

#include <concepts>
#include <functional>
#include <memory>

#include "zenith/core/fwd.hpp"
#include "zenith/core/typedefs.hpp"
#include "zenith/gl/fwd.hpp"
#include "zenith/graphics/fwd.hpp"
#include "zenith/math/quaternion.hpp"
#include "zenith/stl/string.hpp"
#include "zenith/stl/vector.hpp"
#include "zenith/system/fwd.hpp"
#include "zenith/system/input.hpp"
#include "zenith/util/macros.hpp"

namespace zth::debug {

class DebugToolsUi
{
public:
    Key toggle_wireframe_mode_key = Key::F1;

public:
    explicit DebugToolsUi(StringView label = "Debug Tools");
    ZTH_NO_COPY_NO_MOVE(DebugToolsUi)
    ~DebugToolsUi() = default;

    auto on_key_pressed_event(const KeyPressedEvent& event) -> void;
    auto on_update() -> void;

private:
    String _label;
    u32 _frame_rate_limit = 60;
};

class TransformUi
{
public:
    explicit TransformUi(Transformable3D& transformable, StringView label = "Transform");
    ZTH_NO_COPY_NO_MOVE(TransformUi)
    ~TransformUi() = default;

    auto on_update() -> void;

private:
    String _label;
    Transformable3D& _transformable;

    math::Rotation _rotation{};
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

class MaterialUi
{
public:
    explicit MaterialUi(Material& material, StringView label = "Material");
    ZTH_NO_COPY_NO_MOVE(MaterialUi)
    ~MaterialUi() = default;

    auto on_update() -> void;

    auto add_diffuse_map(StringView name, const gl::Texture2D& diffuse_map) -> void;
    auto add_specular_map(StringView name, const gl::Texture2D& specular_map) -> void;
    auto add_emission_map(StringView name, const gl::Texture2D& emission_map) -> void;

private:
    String _label;
    Material& _material;
    usize _material_selected_idx = 0;

    static constexpr i16 _no_map_selected = -1;
    i16 _diffuse_map_selected_idx = _no_map_selected;
    i16 _specular_map_selected_idx = _no_map_selected;
    i16 _emission_map_selected_idx = _no_map_selected;

    Vector<String> _diffuse_map_names;
    Vector<const gl::Texture2D*> _diffuse_maps;
    Vector<String> _specular_map_names;
    Vector<const gl::Texture2D*> _specular_maps;
    Vector<String> _emission_map_names;
    Vector<const gl::Texture2D*> _emission_maps;

private:
    auto set_diffuse_map(i16 idx) -> void;
    auto set_specular_map(i16 idx) -> void;
    auto set_emission_map(i16 idx) -> void;
};

class DirectionalLightUi
{
public:
    explicit DirectionalLightUi(DirectionalLight& light, StringView label = "Directional Light");
    ZTH_NO_COPY_NO_MOVE(DirectionalLightUi)
    ~DirectionalLightUi() = default;

    auto on_update() -> void;

private:
    StringView _label;
    DirectionalLight& _light;
};

class PointLightUi
{
public:
    explicit PointLightUi(PointLight& light, StringView label = "Point Light");
    ZTH_NO_COPY_NO_MOVE(PointLightUi)
    ~PointLightUi() = default;

    auto on_update() -> void;

private:
    String _label;
    PointLight& _light;
};

class SpotLightUi
{
public:
    explicit SpotLightUi(SpotLight& light, StringView label = "Spot Light");
    ZTH_NO_COPY_NO_MOVE(SpotLightUi)
    ~SpotLightUi() = default;

    auto on_update() -> void;

private:
    String _label;
    SpotLight& _light;
};

class ScenePickerUi
{
public:
    Key prev_scene_key = Key::Left;
    Key next_scene_key = Key::Right;

public:
    explicit ScenePickerUi(StringView label = "Scene");
    ZTH_NO_COPY_NO_MOVE(ScenePickerUi)
    ~ScenePickerUi() = default;

    auto on_update() -> void;
    auto on_key_pressed_event(const KeyPressedEvent& event) -> void;

    template<typename T>
    auto add_scene(StringView name)
        requires(std::derived_from<T, Scene>);

    auto prev() -> void;
    auto next() -> void;

private:
    String _label;
    usize _selected_scene_idx = 0;
    usize _scene_count = 0;

    Vector<String> _scene_names;
    Vector<std::function<std::unique_ptr<Scene>()>> _scene_constructors;

private:
    auto load_scene(usize idx) const -> void;
};

template<typename T>
auto ScenePickerUi::add_scene(StringView name)
    requires(std::derived_from<T, Scene>)
{
    _scene_names.emplace_back(name);
    _scene_constructors.emplace_back(std::make_unique<T>);
    _scene_count++;
}

} // namespace zth::debug
