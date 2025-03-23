#pragma once

#include <imgui.h>
#include <ImGuizmo.h>

#include <concepts>
#include <functional>
#include <memory>

#include "zenith/core/fwd.hpp"
#include "zenith/core/typedefs.hpp"
#include "zenith/ecs/entity.hpp"
#include "zenith/ecs/fwd.hpp"
#include "zenith/gl/fwd.hpp"
#include "zenith/renderer/fwd.hpp"
#include "zenith/stl/string.hpp"
#include "zenith/stl/vector.hpp"
#include "zenith/system/fwd.hpp"
#include "zenith/system/input.hpp"
#include "zenith/util/macros.hpp"

namespace zth::debug {

struct TransformGizmo
{
    ImGuizmo::OPERATION operation = ImGuizmo::TRANSLATE;
    ImGuizmo::MODE mode = ImGuizmo::WORLD;

    auto draw(TransformComponent& transform, const PerspectiveCamera& camera) const -> void;
};

struct EntityInspectorPanel
{
    TransformGizmo gizmo;

    auto draw(EntityHandle entity) const -> void;
};

class SceneHierarchyPanel
{
public:
    EntityInspectorPanel inspector;

public:
    explicit SceneHierarchyPanel(Registry& registry);
    ZTH_NO_COPY_NO_MOVE(SceneHierarchyPanel)
    ~SceneHierarchyPanel() = default;

    auto draw() -> void;

private:
    Registry& _registry;
    EntityId _selected_entity_id = null_entity;
};

// @todo: Replace / improve.
class DebugToolsPanel
{
public:
    Key toggle_wireframe_mode_key = Key::F2;

public:
    explicit DebugToolsPanel(StringView label = "Debug Tools");
    ZTH_NO_COPY_NO_MOVE(DebugToolsPanel)
    ~DebugToolsPanel() = default;

    auto draw() -> void;
    auto on_key_pressed_event(const KeyPressedEvent& event) const -> void;

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

    auto draw() -> void;

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
    Key prev_scene_key = Key::Left;
    Key next_scene_key = Key::Right;

public:
    explicit ScenePicker(StringView label = "Scene");
    ZTH_NO_COPY_NO_MOVE(ScenePicker)
    ~ScenePicker() = default;

    auto draw() -> void;
    auto on_key_pressed_event(const KeyPressedEvent& event) -> void;

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
