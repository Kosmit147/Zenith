#include "zenith/core/scene.hpp"

#include <imgui.h>
#include <ImGuizmo.h>

#include "zenith/ecs/components.hpp"
#include "zenith/renderer/light.hpp"
#include "zenith/renderer/renderer.hpp"
#include "zenith/system/event.hpp"

namespace zth {

auto Scene::create_entity(const String& tag) -> EntityHandle
{
    return _registry.create(tag);
}

auto Scene::create_entity(String&& tag) -> EntityHandle
{
    return _registry.create(std::move(tag));
}

auto Scene::dispatch_event(const Event& event) -> void
{
    if (event.type() == EventType::KeyPressed)
    {
        auto [key] = event.key_pressed_event();
        auto& inspector = _scene_hierarchy_panel.inspector;

        // @cleanup: This shouldn't be here.
        if (key == toggle_scene_hierarchy_panel_key)
            scene_hierarchy_panel_enabled = !scene_hierarchy_panel_enabled;

        if (key == switch_to_gizmo_translate_mode_key)
        {
            inspector.gizmo.operation = ImGuizmo::TRANSLATE;
            inspector.gizmo.mode = ImGuizmo::WORLD;
        }

        if (key == switch_to_gizmo_rotate_mode_key)
        {
            inspector.gizmo.operation = ImGuizmo::ROTATE;
            inspector.gizmo.mode = ImGuizmo::LOCAL;
        }

        if (key == switch_to_gizmo_scale_mode_key)
        {
            inspector.gizmo.operation = ImGuizmo::SCALE;
            inspector.gizmo.mode = ImGuizmo::LOCAL;
        }
    }

    auto scripts = _registry.view<ScriptComponent>();

    for (auto&& [entity_id, script] : scripts.each())
        script.script->dispatch_event(EntityHandle{ entity_id, _registry }, event);

    on_event(event);
}

auto Scene::update() -> void
{
    if (scene_hierarchy_panel_enabled)
        _scene_hierarchy_panel.draw();

    auto scripts = _registry.view<ScriptComponent>();

    for (auto&& [entity_id, script] : scripts.each())
        script.script->update(EntityHandle{ entity_id, _registry });

    on_update();
}

auto Scene::render() -> void
{
    Renderer::begin_scene();

    auto lights = _registry.view<const LightComponent>();

    for (auto&& [entity_id, light] : lights.each())
    {
        auto& transform = _registry.get<const TransformComponent>(entity_id);

        switch (light.type())
        {
            using enum LightType;
        case Directional:
            Renderer::submit_directional_light(light.to_directional_light_render_data(transform));
            break;
        case Point:
            Renderer::submit_point_light(light.to_point_light_render_data(transform));
            break;
        case Spot:
            Renderer::submit_spot_light(light.to_spot_light_render_data(transform));
            break;
        case Ambient:
            Renderer::submit_ambient_light(light.to_ambient_light_render_data());
            break;
        }
    }

    auto meshes = _registry.group<const TransformComponent, const MeshComponent, const MaterialComponent>();

    for (auto&& [_, transform, mesh, material] : meshes.each())
        Renderer::submit(*mesh.mesh, transform.transform(), *material.material);

    Renderer::end_scene();
}

} // namespace zth
