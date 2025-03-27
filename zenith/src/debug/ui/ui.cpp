#include "zenith/debug/ui/ui.hpp"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <imgui_stdlib.h>

#include "zenith/core/assert.hpp"
#include "zenith/core/scene_manager.hpp"
#include "zenith/ecs/components.hpp"
#include "zenith/log/format.hpp"
#include "zenith/math/quaternion.hpp"
#include "zenith/memory/temporary_storage.hpp"
#include "zenith/renderer/light.hpp"
#include "zenith/renderer/material.hpp"
#include "zenith/renderer/materials.hpp"
#include "zenith/renderer/renderer.hpp"
#include "zenith/system/event.hpp"
#include "zenith/system/window.hpp"

namespace zth::debug {

namespace {

constexpr auto default_drag_speed = 0.01f;

constexpr auto far_plane_drag_speed = default_drag_speed * 10.0f;

constexpr auto light_attenuation_drag_speed = default_drag_speed * 0.01f;
constexpr auto light_ambient_drag_speed = default_drag_speed * 0.1f;

constexpr auto material_shininess_drag_speed = default_drag_speed * 10.0f;

auto drag_float(const char* label, float& value, float drag_speed = default_drag_speed) -> bool
{
    return ImGui::DragFloat(label, &value, drag_speed);
}

auto drag_float_2(const char* label, float values[2], float drag_speed = default_drag_speed) -> bool
{
    return ImGui::DragFloat2(label, values, drag_speed);
}

auto drag_float_3(const char* label, float values[3], float drag_speed = default_drag_speed) -> bool
{
    return ImGui::DragFloat3(label, values, drag_speed);
}

auto drag_float_4(const char* label, float values[4], float drag_speed = default_drag_speed) -> bool
{
    return ImGui::DragFloat4(label, values, drag_speed);
}

auto drag_vec(const char* label, glm::vec2& vec, float drag_speed = default_drag_speed) -> bool
{
    return drag_float_2(label, glm::value_ptr(vec), drag_speed);
}

auto drag_vec(const char* label, glm::vec3& vec, float drag_speed = default_drag_speed) -> bool
{
    return drag_float_3(label, glm::value_ptr(vec), drag_speed);
}

auto drag_vec(const char* label, glm::vec4& vec, float drag_speed = default_drag_speed) -> bool
{
    return drag_float_4(label, glm::value_ptr(vec), drag_speed);
}

auto drag_angles(const char* label, glm::vec2& angles, float drag_speed = default_drag_speed) -> bool
{
    auto degrees = glm::degrees(angles);

    if (drag_float_2(label, glm::value_ptr(degrees), drag_speed))
    {
        angles = glm::radians(degrees);
        return true;
    }

    return false;
}

auto drag_angles(const char* label, glm::vec3& angles, float drag_speed = default_drag_speed) -> bool
{
    auto degrees = glm::degrees(angles);

    if (drag_float_3(label, glm::value_ptr(degrees), drag_speed))
    {
        angles = glm::radians(degrees);
        return true;
    }

    return false;
}

auto drag_angles(const char* label, glm::vec4& angles, float drag_speed = default_drag_speed) -> bool
{
    auto degrees = glm::degrees(angles);

    if (drag_float_4(label, glm::value_ptr(degrees), drag_speed))
    {
        angles = glm::radians(degrees);
        return true;
    }

    return false;
}

auto slide_float(const char* label, float& value, float min = 0.0f, float max = 1.0f) -> bool
{
    return ImGui::SliderFloat(label, &value, min, max);
}

auto slide_float_2(const char* label, float values[2], float min = 0.0f, float max = 1.0f) -> bool
{
    return ImGui::SliderFloat2(label, values, min, max);
}

auto slide_float_3(const char* label, float values[3], float min = 0.0f, float max = 1.0f) -> bool
{
    return ImGui::SliderFloat3(label, values, min, max);
}

auto slide_float_4(const char* label, float values[4], float min = 0.0f, float max = 1.0f) -> bool
{
    return ImGui::SliderFloat4(label, values, min, max);
}

auto slide_vec(const char* label, glm::vec2& vec, float min = 0.0f, float max = 1.0f) -> bool
{
    return slide_float_2(label, glm::value_ptr(vec), min, max);
}

auto slide_vec(const char* label, glm::vec3& vec, float min = 0.0f, float max = 1.0f) -> bool
{
    return slide_float_3(label, glm::value_ptr(vec), min, max);
}

auto slide_vec(const char* label, glm::vec4& vec, float min = 0.0f, float max = 1.0f) -> bool
{
    return slide_float_4(label, glm::value_ptr(vec), min, max);
}

auto slide_angles(const char* label, glm::vec2& angles, float min_degrees = 0.0f, float max_degrees = 360.0f) -> bool
{
    auto degrees = glm::degrees(angles);

    if (slide_float_2(label, glm::value_ptr(degrees), min_degrees, max_degrees))
    {
        angles = glm::radians(degrees);
        return true;
    }

    return false;
}

auto slide_angles(const char* label, glm::vec3& angles, float min_degrees = 0.0f, float max_degrees = 360.0f) -> bool
{
    auto degrees = glm::degrees(angles);

    if (slide_float_3(label, glm::value_ptr(degrees), min_degrees, max_degrees))
    {
        angles = glm::radians(degrees);
        return true;
    }

    return false;
}

auto slide_angles(const char* label, glm::vec4& angles, float min_degrees = 0.0f, float max_degrees = 360.0f) -> bool
{
    auto degrees = glm::degrees(angles);

    if (slide_float_4(label, glm::value_ptr(degrees), min_degrees, max_degrees))
    {
        angles = glm::radians(degrees);
        return true;
    }

    return false;
}

auto edit_color(const char* label, glm::vec3& color) -> bool
{
    return ImGui::ColorEdit3(label, glm::value_ptr(color));
}

auto edit_color(const char* label, glm::vec4& color) -> bool
{
    return ImGui::ColorEdit4(label, glm::value_ptr(color));
}

auto pick_color(const char* label, glm::vec3& color) -> bool
{
    return ImGui::ColorPicker3(label, glm::value_ptr(color));
}

auto pick_color(const char* label, glm::vec4& color) -> bool
{
    return ImGui::ColorPicker4(label, glm::value_ptr(color));
}

auto slide_angle(const char* label, float& angle_radians, float degrees_min = 0.0f, float degrees_max = 360.0f) -> bool
{
    return ImGui::SliderAngle(label, &angle_radians, degrees_min, degrees_max);
}

auto drag_euler_angles(const char* label, math::EulerAngles& angles, float drag_speed = default_drag_speed) -> bool
{
    auto values = static_cast<glm::vec3>(angles);

    if (drag_angles(label, values, drag_speed))
    {
        angles = math::EulerAngles{ .pitch = values.x, .yaw = values.y, .roll = values.z };
        return true;
    }

    return false;
}

auto edit_quat(const char* label, glm::quat& quaternion) -> bool
{
    return slide_float_4(label, glm::value_ptr(quaternion));
}

auto edit_quat_as_euler_angles(const char* label, glm::quat& quaternion) -> bool
{
    const auto original_angles = math::to_euler_angles(quaternion);
    auto maybe_modified_angles = original_angles;

    if (drag_euler_angles(label, maybe_modified_angles))
    {
        auto change = maybe_modified_angles - original_angles;
        quaternion = math::rotate(quaternion, change);
        return true;
    }

    return false;
}

auto edit_tag(TagComponent& tag) -> void
{
    ImGui::InputText("Tag", &tag.tag);
}

auto edit_transform(TransformComponent& transform) -> void
{
    ImGui::SeparatorText("Transform");

    auto translation = transform.translation();

    if (drag_vec("Translation", translation))
        transform.set_translation(translation);

    auto rotation = transform.rotation();

    if (edit_quat_as_euler_angles("Rotation", rotation))
        transform.set_rotation(rotation);

    auto scale = transform.scale();

    if (drag_vec("Scale", scale))
        transform.set_scale(scale);
}

auto edit_camera(CameraComponent& camera) -> void
{
    ImGui::SeparatorText("Camera");

    drag_float("Aspect Ratio", camera.aspect_ratio);
    slide_angle("FOV", camera.fov, 0.0f, 180.0f);
    drag_float("Near Plane", camera.near);
    drag_float("Far Plane", camera.far, far_plane_drag_speed);
}

auto edit_light_type(LightType& type) -> bool
{
    auto value_changed = false;

    auto light_type_selected_idx = std::to_underlying(type);

    if (ImGui::BeginCombo("Type", to_string(light_type_enumerations[light_type_selected_idx])))
    {
        for (usize i = 0; i < light_type_enumerations.size(); i++)
        {
            const auto is_selected = light_type_selected_idx == i;

            if (ImGui::Selectable(to_string(light_type_enumerations[i]), is_selected))
            {
                light_type_selected_idx = static_cast<decltype(light_type_selected_idx)>(i);
                type = static_cast<LightType>(light_type_selected_idx);
                value_changed = true;
            }

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }

        ImGui::EndCombo();
    }

    return value_changed;
}

auto edit_light_properties(LightProperties& properties) -> void
{
    edit_color("Color", properties.color);
    drag_vec("Ambient", properties.ambient, light_ambient_drag_speed);
    drag_vec("Diffuse", properties.diffuse);
    drag_vec("Specular", properties.specular);
}

auto edit_light_attenuation(LightAttenuation& attenuation) -> void
{
    drag_float("Constant", attenuation.constant, light_attenuation_drag_speed);
    drag_float("Linear", attenuation.linear, light_attenuation_drag_speed);
    drag_float("Quadratic", attenuation.quadratic, light_attenuation_drag_speed);
}

auto edit_directional_light(DirectionalLight& light) -> void
{
    edit_light_properties(light.properties);
}

auto edit_point_light(PointLight& light) -> void
{
    edit_light_properties(light.properties);
    edit_light_attenuation(light.attenuation);
}

auto edit_spot_light(SpotLight& light) -> void
{
    float inner_cutoff = glm::acos(light.inner_cutoff_cosine);
    float outer_cutoff = glm::acos(light.outer_cutoff_cosine);

    if (slide_angle("Inner Cutoff", inner_cutoff))
        light.inner_cutoff_cosine = glm::cos(inner_cutoff);

    if (slide_angle("Outer Cutoff", outer_cutoff))
        light.outer_cutoff_cosine = glm::cos(outer_cutoff);

    edit_light_properties(light.properties);
    edit_light_attenuation(light.attenuation);
}

auto edit_ambient_light(AmbientLight& light) -> void
{
    drag_vec("Ambient", light.ambient, light_ambient_drag_speed);
}

auto edit_light(LightComponent& light) -> void
{
    ImGui::SeparatorText("Light");

    auto light_type = light.type();

    if (edit_light_type(light_type))
        light.set_light(light_type);

    switch (light.type())
    {
        using enum LightType;
    case Directional:
        edit_directional_light(light.directional_light());
        break;
    case Point:
        edit_point_light(light.point_light());
        break;
    case Spot:
        edit_spot_light(light.spot_light());
        break;
    case Ambient:
        edit_ambient_light(light.ambient_light());
        break;
    }
}

auto edit_script(ScriptComponent& script) -> void
{
    ImGui::SeparatorText("Script");
    ImGui::TextUnformatted(script.script().display_name());
}

} // namespace

auto TransformGizmo::draw(TransformComponent& transform) const -> void
{
    ImGuizmo::Enable(true);
    auto transform_matrix = transform.transform();

    auto& view = Renderer::current_camera_view();
    auto& projection = Renderer::current_camera_projection();

    if (ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection), operation, mode,
                             glm::value_ptr(transform_matrix), nullptr, nullptr))
    {
        transform.set_transform(transform_matrix);
    }
}

auto EntityInspectorPanel::draw(EntityHandle entity) const -> void
{
    ZTH_ASSERT(entity.valid());

    ImGui::Begin("Entity Inspector");

    {
        // TagComponent and TransformComponent are mandatory.
        static_assert(IsIntegralComponent<TagComponent>);
        static_assert(IsIntegralComponent<TransformComponent>);

        auto& tag = entity.get<TagComponent>();
        edit_tag(tag);

        auto& transform = entity.get<TransformComponent>();
        edit_transform(transform);

        if (Window::cursor_enabled())
            gizmo.draw(transform);
    }

    if (entity.any_of<CameraComponent>())
    {
        auto& camera = entity.get<CameraComponent>();
        edit_camera(camera);
    }

    if (entity.any_of<LightComponent>())
    {
        auto& light = entity.get<LightComponent>();
        edit_light(light);
    }

    if (entity.any_of<MeshComponent>())
    {
        // auto& mesh = entity.get<MeshComponent>();
        // @todo
    }

    if (entity.any_of<MaterialComponent>())
    {
        // auto& material = entity.get<MaterialComponent>();
        // @todo
    }

    if (entity.any_of<ScriptComponent>())
    {
        auto& script = entity.get<ScriptComponent>();
        edit_script(script);
    }

    ImGui::End();
}

SceneHierarchyPanel::SceneHierarchyPanel(Registry& registry) : _registry(registry) {}

auto SceneHierarchyPanel::draw() -> void
{
    ImGui::Begin("Scene Hierarchy");

    for (auto entity_id : _registry.view<EntityId>())
    {
        const auto& tag = _registry.get<const TagComponent>(entity_id);

        auto label = format_to_temporary("{}##{}", tag.tag, std::to_underlying(entity_id));

        if (ImGui::Selectable(label.c_str(), _selected_entity_id == entity_id))
            _selected_entity_id = entity_id;
    }

    if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
        _selected_entity_id = null_entity;
    else if (_registry.valid(_selected_entity_id))
        inspector.draw(EntityHandle{ _selected_entity_id, _registry });

    ImGui::End();
}

DebugToolsPanel::DebugToolsPanel(StringView label) : _label(label) {}

auto DebugToolsPanel::draw() -> void
{
    ImGui::Begin(_label.data());

    auto fps = ImGui::GetIO().Framerate;
    ImGui::Text("FPS: %0.0f", fps);

    bool frame_rate_limit_enabled;

    {
        auto limit = Window::frame_rate_limit();
        frame_rate_limit_enabled = limit.has_value();
        _frame_rate_limit = limit.value_or(_frame_rate_limit);
    }

    if (ImGui::Checkbox("FPS Limit", &frame_rate_limit_enabled))
    {
        if (frame_rate_limit_enabled)
            Window::set_frame_rate_limit(_frame_rate_limit);
        else
            Window::disable_frame_rate_limit();
    }

    if (frame_rate_limit_enabled)
    {
        static_assert(sizeof(int) == sizeof(_frame_rate_limit));

        if (ImGui::InputInt("##", reinterpret_cast<int*>(&_frame_rate_limit)))
            Window::set_frame_rate_limit(_frame_rate_limit);
    }

    auto label = format_to_temporary("Wireframe ({})", toggle_wireframe_mode_key);
    auto wireframe_mode_enabled = Renderer::wireframe_mode_enabled();

    if (ImGui::Checkbox(label.c_str(), &wireframe_mode_enabled))
        Renderer::set_wireframe_mode(wireframe_mode_enabled);

    ImGui::End();
}

auto DebugToolsPanel::on_key_pressed_event(const KeyPressedEvent& event) const -> void
{
    if (event.key == toggle_wireframe_mode_key)
        Renderer::toggle_wireframe_mode();
}

MaterialPanel::MaterialPanel(Material& material, StringView label) : _label(label), _material(material) {}

auto MaterialPanel::draw() -> void
{
    const auto& materials = materials::materials();
    const auto& material_names = materials::material_names;

    ImGui::Begin(_label.data());

    if (ImGui::BeginCombo("Preset", material_names[_material_selected_idx]))
    {
        for (usize i = 0; i < material_names.size(); i++)
        {
            const auto is_selected = _material_selected_idx == i;

            if (ImGui::Selectable(material_names[i], is_selected))
            {
                _material_selected_idx = i;
                _diffuse_map_selected_idx = -1;
                _specular_map_selected_idx = -1;
                _emission_map_selected_idx = -1;
                _material = materials[_material_selected_idx];
            }

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }

        ImGui::EndCombo();
    }

    pick_color("Albedo", _material.albedo);

    auto map_picker = [](StringView label, i16 selected_idx, const Vector<String>& map_names) {
        constexpr auto none_selected_label = "None";
        Optional<i16> pick = nil;

        StringView selected_map_name = none_selected_label;

        if (selected_idx >= 0)
            selected_map_name = map_names[selected_idx];

        if (ImGui::BeginCombo(label.data(), selected_map_name.data()))
        {
            {
                const auto is_selected = selected_idx == _no_map_selected;

                if (ImGui::Selectable(none_selected_label, is_selected))
                    pick = _no_map_selected;

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }

            for (i16 i = 0; i < static_cast<i16>(map_names.size()); i++)
            {
                const auto is_selected = selected_idx == i;

                if (ImGui::Selectable(map_names[i].c_str(), is_selected))
                    pick = i;

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }

        return pick;
    };

    if (auto pick = map_picker("Diffuse Map", _diffuse_map_selected_idx, _diffuse_map_names))
        set_diffuse_map(*pick);

    if (auto pick = map_picker("Specular Map", _specular_map_selected_idx, _specular_map_names))
        set_specular_map(*pick);

    if (auto pick = map_picker("Emission Map", _emission_map_selected_idx, _emission_map_names))
        set_emission_map(*pick);

    drag_vec("Ambient", _material.ambient, light_ambient_drag_speed);
    drag_vec("Diffuse", _material.diffuse);
    drag_vec("Specular", _material.specular);
    drag_float("Shininess", _material.shininess, material_shininess_drag_speed);

    ImGui::End();
}

auto MaterialPanel::add_diffuse_map(StringView name, const gl::Texture2D& diffuse_map) -> void
{
    _diffuse_map_names.emplace_back(name);
    _diffuse_maps.push_back(&diffuse_map);
}

auto MaterialPanel::add_specular_map(StringView name, const gl::Texture2D& specular_map) -> void
{
    _specular_map_names.emplace_back(name);
    _specular_maps.push_back(&specular_map);
}

auto MaterialPanel::add_emission_map(StringView name, const gl::Texture2D& emission_map) -> void
{
    _emission_map_names.emplace_back(name);
    _emission_maps.push_back(&emission_map);
}

auto MaterialPanel::set_diffuse_map(i16 idx) -> void
{
    _diffuse_map_selected_idx = idx;
    _material.diffuse_map = idx >= 0 ? _diffuse_maps[idx] : nullptr;
}

auto MaterialPanel::set_specular_map(i16 idx) -> void
{
    _specular_map_selected_idx = idx;
    _material.specular_map = idx >= 0 ? _specular_maps[idx] : nullptr;
}

auto MaterialPanel::set_emission_map(i16 idx) -> void
{
    _emission_map_selected_idx = idx;
    _material.emission_map = idx >= 0 ? _emission_maps[idx] : nullptr;
}

ScenePicker::ScenePicker(StringView label) : _label(label) {}

auto ScenePicker::draw() -> void
{
    ImGui::Begin(_label.data());

    ImGui::Text("%s", _scene_names[_selected_scene_idx].c_str());

    auto prev_scene_label = format_to_temporary("Prev ({})", prev_scene_key);
    ImGui::TextUnformatted(prev_scene_label.c_str());

    ImGui::SameLine();

    if (ImGui::ArrowButton("Prev", ImGuiDir_Left))
        prev_scene();

    ImGui::SameLine();

    if (ImGui::ArrowButton("Next", ImGuiDir_Right))
        next_scene();

    ImGui::SameLine();

    auto next_scene_label = format_to_temporary("Next ({})", next_scene_key);
    ImGui::TextUnformatted(next_scene_label.c_str());

    ImGui::End();
}

auto ScenePicker::on_key_pressed_event(const KeyPressedEvent& event) -> void
{
    if (event.key == prev_scene_key)
        prev_scene();
    else if (event.key == next_scene_key)
        next_scene();
}

auto ScenePicker::prev_scene() -> void
{
    if (_scene_count == 0)
        return;

    if (_selected_scene_idx == 0)
        _selected_scene_idx = _scene_count - 1;
    else
        _selected_scene_idx--;

    load_scene(_selected_scene_idx);
}

auto ScenePicker::next_scene() -> void
{
    if (_scene_count == 0)
        return;

    if (_selected_scene_idx >= _scene_count - 1)
        _selected_scene_idx = 0;
    else
        _selected_scene_idx++;

    load_scene(_selected_scene_idx);
}

auto ScenePicker::load_scene(usize idx) const -> void
{
    auto scene = _scene_constructors[idx]();
    SceneManager::load_scene(std::move(scene));
}

} // namespace zth::debug
