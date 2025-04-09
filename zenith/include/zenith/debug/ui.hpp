#pragma once

#include <imgui.h>
#include <ImGuizmo.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <concepts>
#include <functional>
#include <memory>

#include "zenith/core/fwd.hpp"
#include "zenith/core/typedefs.hpp"
#include "zenith/ecs/ecs.hpp"
#include "zenith/ecs/fwd.hpp"
#include "zenith/gl/fwd.hpp"
#include "zenith/math/quaternion.hpp"
#include "zenith/renderer/fwd.hpp"
#include "zenith/stl/string.hpp"
#include "zenith/stl/vector.hpp"
#include "zenith/system/fwd.hpp"
#include "zenith/util/macros.hpp"

namespace zth::debug {

constexpr auto default_ui_drag_speed = 0.01f;

auto drag_float(const char* label, float& value, float drag_speed = default_ui_drag_speed) -> bool;
auto drag_float_2(const char* label, float values[2], float drag_speed = default_ui_drag_speed) -> bool;
auto drag_float_3(const char* label, float values[3], float drag_speed = default_ui_drag_speed) -> bool;
auto drag_float_4(const char* label, float values[4], float drag_speed = default_ui_drag_speed) -> bool;

auto drag_vec(const char* label, glm::vec2& vec, float drag_speed = default_ui_drag_speed) -> bool;
auto drag_vec(const char* label, glm::vec3& vec, float drag_speed = default_ui_drag_speed) -> bool;
auto drag_vec(const char* label, glm::vec4& vec, float drag_speed = default_ui_drag_speed) -> bool;

auto drag_angles(const char* label, glm::vec2& angles, float drag_speed = default_ui_drag_speed) -> bool;
auto drag_angles(const char* label, glm::vec3& angles, float drag_speed = default_ui_drag_speed) -> bool;
auto drag_angles(const char* label, glm::vec4& angles, float drag_speed = default_ui_drag_speed) -> bool;

auto slide_float(const char* label, float& value, float min = 0.0f, float max = 1.0f) -> bool;
auto slide_float_2(const char* label, float values[2], float min = 0.0f, float max = 1.0f) -> bool;
auto slide_float_3(const char* label, float values[3], float min = 0.0f, float max = 1.0f) -> bool;
auto slide_float_4(const char* label, float values[4], float min = 0.0f, float max = 1.0f) -> bool;

auto slide_vec(const char* label, glm::vec2& vec, float min = 0.0f, float max = 1.0f) -> bool;
auto slide_vec(const char* label, glm::vec3& vec, float min = 0.0f, float max = 1.0f) -> bool;
auto slide_vec(const char* label, glm::vec4& vec, float min = 0.0f, float max = 1.0f) -> bool;

auto slide_angles(const char* label, glm::vec2& angles, float min_degrees = 0.0f, float max_degrees = 360.0f) -> bool;
auto slide_angles(const char* label, glm::vec3& angles, float min_degrees = 0.0f, float max_degrees = 360.0f) -> bool;
auto slide_angles(const char* label, glm::vec4& angles, float min_degrees = 0.0f, float max_degrees = 360.0f) -> bool;

auto checkbox(const char* label, bool& value) -> bool;

auto edit_color(const char* label, glm::vec3& color) -> bool;
auto edit_color(const char* label, glm::vec4& color) -> bool;
auto pick_color(const char* label, glm::vec3& color) -> bool;
auto pick_color(const char* label, glm::vec4& color) -> bool;

auto slide_angle(const char* label, float& angle_radians, float degrees_min = 0.0f, float degrees_max = 360.0f) -> bool;
auto drag_euler_angles(const char* label, math::EulerAngles& angles, float drag_speed = default_ui_drag_speed) -> bool;

auto edit_quat(const char* label, glm::quat& quaternion) -> bool;
auto edit_quat_as_euler_angles(const char* label, glm::quat& quaternion) -> bool;

auto select_key(const char* label, Key& key) -> bool;
auto select_mouse_button(const char* label, MouseButton& button) -> bool;

auto select_light_type(LightType& type) -> bool;
auto edit_light_properties(LightProperties& properties) -> void;
auto edit_light_attenuation(LightAttenuation& attenuation) -> void;
auto edit_directional_light(DirectionalLight& light) -> void;
auto edit_point_light(PointLight& light) -> void;
auto edit_spot_light(SpotLight& light) -> void;
auto edit_ambient_light(AmbientLight& light) -> void;

auto edit_component(TagComponent& tag) -> void;
auto edit_component(TransformComponent& transform) -> void;
auto edit_component(CameraComponent& camera) -> void;
auto edit_component(LightComponent& light) -> void;
auto edit_component(MeshComponent& mesh) -> void;
auto edit_component(MaterialComponent& material) -> void;
auto edit_component(ScriptComponent& script) -> void;

struct TransformGizmo
{
    ImGuizmo::OPERATION operation = ImGuizmo::TRANSLATE;
    ImGuizmo::MODE mode = ImGuizmo::WORLD;

    auto display(TransformComponent& transform) const -> void;
};

struct EntityInspectorPanel
{
    TransformGizmo gizmo;

    auto display(EntityHandle entity) const -> void;
};

class SceneHierarchyPanel
{
public:
    EntityInspectorPanel inspector;

public:
    explicit SceneHierarchyPanel() = default;
    ZTH_NO_COPY_NO_MOVE(SceneHierarchyPanel)
    ~SceneHierarchyPanel() = default;

    auto display(Registry& registry) -> void;

private:
    EntityId _selected_entity_id = null_entity;
    String _search;
};

// @todo: Replace / improve.
class DebugPanel
{
public:
    explicit DebugPanel(StringView label = "Debug");
    ZTH_NO_COPY_NO_MOVE(DebugPanel)
    ~DebugPanel() = default;

    auto display() -> void;

private:
    String _label;
    u32 _frame_rate_limit = 60;
};

// @todo: Replace / improve.
class MaterialPanel
{
public:
    explicit MaterialPanel(Material& material, StringView label = "Material");
    ZTH_NO_COPY_NO_MOVE(MaterialPanel)
    ~MaterialPanel() = default;

    auto display() -> void;

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

// @todo: Replace / improve.
class ScenePicker
{
public:
    explicit ScenePicker(StringView label = "Scene");
    ZTH_NO_COPY_NO_MOVE(ScenePicker)
    ~ScenePicker() = default;

    auto display() -> void;

    template<typename T>
    auto add_scene(StringView name)
        requires(std::derived_from<T, Scene>);

    auto prev_scene() -> void;
    auto next_scene() -> void;

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
auto ScenePicker::add_scene(StringView name)
    requires(std::derived_from<T, Scene>)
{
    _scene_names.emplace_back(name);
    _scene_constructors.emplace_back(std::make_unique<T>);
    _scene_count++;
}

} // namespace zth::debug
