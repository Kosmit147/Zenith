#pragma once

#include <imgui.h>
#include <ImGuizmo.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <spdlog/fmt/fmt.h>

#include <concepts>
#include <functional>
#include <utility>

#include "zenith/core/fwd.hpp"
#include "zenith/core/typedefs.hpp"
#include "zenith/ecs/ecs.hpp"
#include "zenith/ecs/fwd.hpp"
#include "zenith/math/geometry.hpp"
#include "zenith/math/quaternion.hpp"
#include "zenith/memory/managed.hpp"
#include "zenith/memory/temporary_storage.hpp"
#include "zenith/renderer/fwd.hpp"
#include "zenith/stl/string.hpp"
#include "zenith/stl/vector.hpp"
#include "zenith/system/fwd.hpp"
#include "zenith/util/macros.hpp"

namespace zth::debug {

constexpr inline auto default_float_drag_speed = 0.01f;
constexpr inline auto default_int_drag_speed = 1.0f;

auto text(const char* txt) -> void;
auto text(StringView txt) -> void;
auto text(const String& txt) -> void;
template<typename... Args> auto text(fmt::format_string<Args...> fmt, Args&&... args) -> void;

auto drag_int(const char* label, i32& value, float drag_speed = default_int_drag_speed) -> bool;
auto drag_int_2(const char* label, i32 values[2], float drag_speed = default_int_drag_speed) -> bool;
auto drag_int_3(const char* label, i32 values[3], float drag_speed = default_int_drag_speed) -> bool;
auto drag_int_4(const char* label, i32 values[4], float drag_speed = default_int_drag_speed) -> bool;

auto drag_uint(const char* label, u32& value, float drag_speed = default_int_drag_speed) -> bool;
auto drag_uint_2(const char* label, u32 values[2], float drag_speed = default_int_drag_speed) -> bool;
auto drag_uint_3(const char* label, u32 values[3], float drag_speed = default_int_drag_speed) -> bool;
auto drag_uint_4(const char* label, u32 values[4], float drag_speed = default_int_drag_speed) -> bool;

auto drag_float(const char* label, float& value, float drag_speed = default_float_drag_speed) -> bool;
auto drag_float_2(const char* label, float values[2], float drag_speed = default_float_drag_speed) -> bool;
auto drag_float_3(const char* label, float values[3], float drag_speed = default_float_drag_speed) -> bool;
auto drag_float_4(const char* label, float values[4], float drag_speed = default_float_drag_speed) -> bool;

auto drag_vec(const char* label, glm::uvec2& vec, float drag_speed = default_int_drag_speed) -> bool;
auto drag_vec(const char* label, glm::uvec3& vec, float drag_speed = default_int_drag_speed) -> bool;
auto drag_vec(const char* label, glm::uvec4& vec, float drag_speed = default_int_drag_speed) -> bool;

auto drag_vec(const char* label, glm::vec2& vec, float drag_speed = default_float_drag_speed) -> bool;
auto drag_vec(const char* label, glm::vec3& vec, float drag_speed = default_float_drag_speed) -> bool;
auto drag_vec(const char* label, glm::vec4& vec, float drag_speed = default_float_drag_speed) -> bool;

auto drag_angles(const char* label, glm::vec2& angles, float drag_speed = default_float_drag_speed) -> bool;
auto drag_angles(const char* label, glm::vec3& angles, float drag_speed = default_float_drag_speed) -> bool;
auto drag_angles(const char* label, glm::vec4& angles, float drag_speed = default_float_drag_speed) -> bool;

auto slide_int(const char* label, i32& value, i32 min, i32 max) -> bool;
auto slide_int_2(const char* label, i32 values[2], i32 min, i32 max) -> bool;
auto slide_int_3(const char* label, i32 values[3], i32 min, i32 max) -> bool;
auto slide_int_4(const char* label, i32 values[4], i32 min, i32 max) -> bool;

auto slide_uint(const char* label, u32& value, u32 min, u32 max) -> bool;
auto slide_uint_2(const char* label, u32 values[2], u32 min, u32 max) -> bool;
auto slide_uint_3(const char* label, u32 values[3], u32 min, u32 max) -> bool;
auto slide_uint_4(const char* label, u32 values[4], u32 min, u32 max) -> bool;

auto slide_float(const char* label, float& value, float min = 0.0f, float max = 1.0f) -> bool;
auto slide_float_2(const char* label, float values[2], float min = 0.0f, float max = 1.0f) -> bool;
auto slide_float_3(const char* label, float values[3], float min = 0.0f, float max = 1.0f) -> bool;
auto slide_float_4(const char* label, float values[4], float min = 0.0f, float max = 1.0f) -> bool;

auto slide_vec(const char* label, glm::uvec2& vec, u32 min, u32 max) -> bool;
auto slide_vec(const char* label, glm::uvec3& vec, u32 min, u32 max) -> bool;
auto slide_vec(const char* label, glm::uvec4& vec, u32 min, u32 max) -> bool;

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

auto input_int(const char* label, i32& value) -> bool;
auto input_uint(const char* label, u32& value) -> bool;
auto input_text(const char* label, String& value) -> bool;

auto drag_rect(const char* label, Rect<u32>& rect, float drag_speed = default_int_drag_speed) -> bool;
auto drag_rect_bounds(const char* label, RectBounds<u32>& bounds, float drag_speed = default_int_drag_speed) -> bool;

auto slide_rect(const char* label, Rect<u32>& rect, Rect<u32> min, Rect<u32> max) -> bool;
auto slide_rect_bounds(const char* label, RectBounds<u32>& bounds, RectBounds<u32> min, RectBounds<u32> max) -> bool;

auto slide_angle(const char* label, float& angle_radians, float degrees_min = 0.0f, float degrees_max = 360.0f) -> bool;
auto drag_euler_angles(const char* label, math::EulerAngles& angles, float drag_speed = default_float_drag_speed)
    -> bool;

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
auto edit_component(Sprite2DComponent& sprite) -> void;
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

class ScenePicker
{
public:
    explicit ScenePicker(StringView label = "Scene");
    ZTH_NO_COPY_NO_MOVE(ScenePicker)
    ~ScenePicker() = default;

    auto display() -> void;

    template<std::derived_from<Scene> T> auto add_scene(StringView name);

    auto prev_scene() -> void;
    auto next_scene() -> void;

private:
    String _label;
    usize _selected_scene_idx = 0;
    usize _scene_count = 0;

    Vector<String> _scene_names;
    Vector<std::function<UniquePtr<Scene>()>> _scene_factories;

private:
    auto load_scene(usize idx) const -> void;
};

template<typename... Args> auto text(fmt::format_string<Args...> fmt, Args&&... args) -> void
{
    text(format_to_temporary(fmt, std::forward<decltype(args)>(args)...));
}

template<std::derived_from<Scene> T> auto ScenePicker::add_scene(StringView name)
{
    _scene_names.emplace_back(name);
    _scene_factories.emplace_back(make_unique<T>);
    _scene_count++;
}

} // namespace zth::debug
