#pragma once

#include <entt/entity/registry.hpp>

#include "zenith/debug/ui/ui.hpp"
#include "zenith/ecs/entity.hpp"
#include "zenith/stl/string.hpp"
#include "zenith/system/fwd.hpp"
#include "zenith/util/macros.hpp"

namespace zth {

class Scene
{
public:
    // @cleanup: This stuff shouldn't be in Scene.
    Key toggle_scene_hierarchy_panel_key = Key::F1;
    Key switch_to_gizmo_translate_mode_key = Key::Q;
    Key switch_to_gizmo_rotate_mode_key = Key::E;
    Key switch_to_gizmo_scale_mode_key = Key::R;

    bool scene_hierarchy_panel_enabled = false;

public:
    explicit Scene() = default;
    ZTH_NO_COPY_NO_MOVE(Scene)
    virtual ~Scene() = default;

    friend class SceneManager;

protected:
    // @todo: Wrap registry and entity in our own classes.
    [[nodiscard]] auto registry(this auto&& self) -> auto& { return self._registry; }
    [[nodiscard]] auto create_entity(const String& tag) -> Entity;
    [[nodiscard]] auto create_entity(String&& tag) -> Entity;

private:
    entt::registry _registry;
    // @cleanup: SceneHierarchyPanel shouldn't be in Scene.
    debug::SceneHierarchyPanel _scene_hierarchy_panel{ _registry };

private:
    auto update() -> void;
    auto dispatch_event(const Event& event) -> void;
    auto render() -> void;

    virtual auto on_load() -> void {}
    virtual auto on_update() -> void {}
    virtual auto on_event([[maybe_unused]] const Event& event) -> void {}
    virtual auto on_render() -> void {}
    virtual auto on_unload() -> void {}
};

} // namespace zth
