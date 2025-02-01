#pragma once

#include <imgui.h>
#include <ImGuizmo.h>
#include <glm/vec3.hpp>

#include <concepts>
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "zenith/core/fwd.hpp"
#include "zenith/core/typedefs.hpp"
#include "zenith/gl/fwd.hpp"
#include "zenith/graphics/fwd.hpp"
#include "zenith/math/quaternion.hpp"
#include "zenith/system/fwd.hpp"
#include "zenith/system/input.hpp"
#include "zenith/util/macros.hpp"

namespace zth::debug {

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
    u32 _frame_rate_limit = 60;
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
    explicit MaterialUi(Material& material);
    ZTH_NO_COPY_NO_MOVE(MaterialUi)
    ~MaterialUi() = default;

    auto on_update() -> void;

    auto add_diffuse_map(std::string_view name, const gl::Texture2D& diffuse_map) -> void;
    auto add_specular_map(std::string_view name, const gl::Texture2D& specular_map) -> void;
    auto add_emission_map(std::string_view name, const gl::Texture2D& emission_map) -> void;

private:
    Material& _material;
    usize _material_selected_idx = 0;

    static constexpr i16 _no_map_selected = -1;
    i16 _diffuse_map_selected_idx = _no_map_selected;
    i16 _specular_map_selected_idx = _no_map_selected;
    i16 _emission_map_selected_idx = _no_map_selected;

    std::vector<std::string> _diffuse_map_names;
    std::vector<const gl::Texture2D*> _diffuse_maps;
    std::vector<std::string> _specular_map_names;
    std::vector<const gl::Texture2D*> _specular_maps;
    std::vector<std::string> _emission_map_names;
    std::vector<const gl::Texture2D*> _emission_maps;

private:
    auto set_diffuse_map(i16 idx) -> void;
    auto set_specular_map(i16 idx) -> void;
    auto set_emission_map(i16 idx) -> void;
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

class SpotLightUi
{
public:
    explicit SpotLightUi(SpotLight& light);
    ZTH_NO_COPY_NO_MOVE(SpotLightUi)
    ~SpotLightUi() = default;

    auto on_update() -> void;

private:
    SpotLight& _light;
};

class ScenePickerUi
{
public:
    Key prev_scene_key = Key::Left;
    Key next_scene_key = Key::Right;

public:
    explicit ScenePickerUi() = default;
    ZTH_NO_COPY_NO_MOVE(ScenePickerUi)
    ~ScenePickerUi() = default;

    auto on_update() -> void;
    auto on_key_pressed_event(const KeyPressedEvent& event) -> void;

    template<typename T>
    auto add_scene(std::string_view name)
        requires(std::derived_from<T, Scene>);

    auto prev() -> void;
    auto next() -> void;

private:
    usize _selected_scene_idx = 0;
    usize _scene_count = 0;

    std::vector<std::string> _scene_names;
    std::vector<std::function<std::unique_ptr<Scene>()>> _scene_constructors;

private:
    auto load_scene(usize idx) const -> void;
};

template<typename T>
auto ScenePickerUi::add_scene(std::string_view name)
    requires(std::derived_from<T, Scene>)
{
    _scene_names.emplace_back(name);
    _scene_constructors.emplace_back(std::make_unique<T>);
    _scene_count++;
}

} // namespace zth::debug
