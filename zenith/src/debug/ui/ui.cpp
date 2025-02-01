#include "zenith/debug/ui/ui.hpp"

#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <spdlog/fmt/fmt.h>

#include "zenith/core/scene_manager.hpp"
#include "zenith/core/transform.hpp"
#include "zenith/graphics/light.hpp"
#include "zenith/graphics/material.hpp"
#include "zenith/graphics/materials.hpp"
#include "zenith/graphics/renderer.hpp"
#include "zenith/log/format.hpp"
#include "zenith/system/event.hpp"
#include "zenith/system/window.hpp"

namespace zth::debug {

namespace {

constexpr auto slider_drag_speed = 0.01f;

} // namespace

auto DebugToolsUi::on_key_pressed_event(const KeyPressedEvent& event) -> void
{
    if (event.key == toggle_wireframe_mode_key)
        Renderer::toggle_wireframe_mode();
}

auto DebugToolsUi::on_update() -> void
{
    ImGui::Begin("Debug Tools");

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
        if (ImGui::InputInt("##", reinterpret_cast<int*>(&_frame_rate_limit)))
            Window::set_frame_rate_limit(_frame_rate_limit);
    }

    auto label = ZTH_FORMAT("Wireframe ({})", toggle_wireframe_mode_key);
    auto wireframe_mode_enabled = Renderer::wireframe_mode_enabled();

    if (ImGui::Checkbox(label.c_str(), &wireframe_mode_enabled))
        Renderer::set_wireframe_mode(wireframe_mode_enabled);

    ImGui::End();
}

TransformUi::TransformUi(Transformable3D& transformable) : _transformable(transformable) {}

auto TransformUi::on_update() -> void
{
    ImGui::Begin("Transform");

    auto translation = _transformable.translation();

    if (ImGui::DragFloat3("Translation", reinterpret_cast<float*>(&translation), slider_drag_speed))
        _transformable.set_translation(translation);

    if (ImGui::SliderAngle("Rotation Angle", &_rotation.angle))
        _transformable.set_rotation(_rotation);

    if (ImGui::DragFloat3("Rotation Axis", reinterpret_cast<float*>(&_rotation.axis), slider_drag_speed))
    {
        _rotation.axis = glm::normalize(_rotation.axis);
        _transformable.set_rotation(_rotation);
    }

    auto scale = _transformable.scale();

    if (_uniform_scale)
    {
        if (ImGui::DragFloat("Scale", reinterpret_cast<float*>(&scale), slider_drag_speed))
        {
            scale = glm::vec3{ scale.x };
            _transformable.set_scale(scale);
        }
    }
    else
    {
        if (ImGui::DragFloat3("Scale", reinterpret_cast<float*>(&scale), slider_drag_speed))
            _transformable.set_scale(scale);
    }

    if (ImGui::Checkbox("Uniform Scale", &_uniform_scale))
    {
        if (_uniform_scale)
        {
            scale = glm::vec3{ scale.x };
            _transformable.set_scale(scale);
        }
    }

    ImGui::End();
}

TransformGizmo::TransformGizmo(Transformable3D& transformable, const PerspectiveCamera& camera)
    : _transformable(transformable), _camera(camera)
{}

auto TransformGizmo::on_update() -> void
{
    if (!enabled)
        return;

    ImGuizmo::Enable(true);
    auto transform = _transformable.transform();

    auto& view = _camera.view();
    auto& projection = _camera.projection();

    if (ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection), _current_gizmo_operation,
                             _current_gizmo_mode, glm::value_ptr(transform), nullptr, nullptr))
    {
        _transformable.set_transform(transform);
    }
}

auto TransformGizmo::on_key_pressed_event(const KeyPressedEvent& event) -> void
{
    if (event.key == toggle_key)
    {
        enabled = !enabled;
    }
    else if (event.key == switch_to_translate_mode_key)
    {
        _current_gizmo_operation = ImGuizmo::TRANSLATE;
        _current_gizmo_mode = ImGuizmo::WORLD;
    }
    else if (event.key == switch_to_rotate_mode_key)
    {
        _current_gizmo_operation = ImGuizmo::ROTATE;
        _current_gizmo_mode = ImGuizmo::LOCAL;
    }
    else if (event.key == switch_to_scale_mode_key)
    {
        _current_gizmo_operation = ImGuizmo::SCALE;
        _current_gizmo_mode = ImGuizmo::LOCAL;
    }
}

MaterialUi::MaterialUi(Material& material) : _material(material) {}

auto MaterialUi::on_update() -> void
{
    const auto& materials = materials::materials();
    const auto& material_names = materials::material_names;

    ImGui::Begin("Material");

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

    ImGui::ColorPicker3("Albedo", reinterpret_cast<float*>(&_material.albedo));

    auto map_picker = [](std::string_view label, i16 selected_idx, const std::vector<std::string>& map_names) {
        constexpr auto none_selected_label = "None";
        std::optional<i16> pick = std::nullopt;

        std::string_view selected_map_name = none_selected_label;

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

    ImGui::DragFloat3("Ambient", reinterpret_cast<float*>(&_material.ambient), slider_drag_speed * 0.1f);
    ImGui::DragFloat3("Diffuse", reinterpret_cast<float*>(&_material.diffuse), slider_drag_speed);
    ImGui::DragFloat3("Specular", reinterpret_cast<float*>(&_material.specular), slider_drag_speed);
    ImGui::DragFloat("Shininess", &_material.shininess, slider_drag_speed * 10.0f);

    ImGui::End();
}

auto MaterialUi::add_diffuse_map(std::string_view name, const gl::Texture2D& diffuse_map) -> void
{
    _diffuse_map_names.emplace_back(name);
    _diffuse_maps.push_back(&diffuse_map);
}

auto MaterialUi::add_specular_map(std::string_view name, const gl::Texture2D& specular_map) -> void
{
    _specular_map_names.emplace_back(name);
    _specular_maps.push_back(&specular_map);
}

auto MaterialUi::add_emission_map(std::string_view name, const gl::Texture2D& emission_map) -> void
{
    _emission_map_names.emplace_back(name);
    _emission_maps.push_back(&emission_map);
}

auto MaterialUi::set_diffuse_map(i16 idx) -> void
{
    _diffuse_map_selected_idx = idx;
    _material.diffuse_map = idx >= 0 ? _diffuse_maps[idx] : nullptr;
}

auto MaterialUi::set_specular_map(i16 idx) -> void
{
    _specular_map_selected_idx = idx;
    _material.specular_map = idx >= 0 ? _specular_maps[idx] : nullptr;
}

auto MaterialUi::set_emission_map(i16 idx) -> void
{
    _emission_map_selected_idx = idx;
    _material.emission_map = idx >= 0 ? _emission_maps[idx] : nullptr;
}

// @cleanup: move sliders for light properties and attenuation into separate functions

DirectionalLightUi::DirectionalLightUi(DirectionalLight& light) : _light(light) {}

auto DirectionalLightUi::on_update() -> void
{
    ImGui::Begin("Directional Light");

    if (ImGui::SliderFloat3("Direction", reinterpret_cast<float*>(&_light.direction), -1.0f, 1.0f))
        _light.direction = glm::normalize(_light.direction);

    // @cleanup: duplicated code
    ImGui::ColorPicker3("Color", reinterpret_cast<float*>(&_light.properties.color));
    ImGui::DragFloat3("Ambient", reinterpret_cast<float*>(&_light.properties.ambient), slider_drag_speed * 0.1f);
    ImGui::DragFloat3("Diffuse", reinterpret_cast<float*>(&_light.properties.diffuse), slider_drag_speed);
    ImGui::DragFloat3("Specular", reinterpret_cast<float*>(&_light.properties.specular), slider_drag_speed);

    ImGui::End();
}

PointLightUi::PointLightUi(PointLight& light) : _light(light) {}

auto PointLightUi::on_update() -> void
{
    ImGui::Begin("Point Light");

    // @cleanup: duplicated code
    ImGui::DragFloat3("Position", reinterpret_cast<float*>(&_light.position), slider_drag_speed * 0.1f);
    ImGui::ColorPicker3("Color", reinterpret_cast<float*>(&_light.properties.color));
    ImGui::DragFloat3("Ambient", reinterpret_cast<float*>(&_light.properties.ambient), slider_drag_speed * 0.1f);
    ImGui::DragFloat3("Diffuse", reinterpret_cast<float*>(&_light.properties.diffuse), slider_drag_speed);
    ImGui::DragFloat3("Specular", reinterpret_cast<float*>(&_light.properties.specular), slider_drag_speed);

    static_assert(sizeof(_light.attenuation) == 3 * sizeof(float));
    ImGui::DragFloat3("Attenuation", reinterpret_cast<float*>(&_light.attenuation), slider_drag_speed * 0.01f);

    ImGui::End();
}

SpotLightUi::SpotLightUi(SpotLight& light) : _light(light) {}

auto SpotLightUi::on_update() -> void
{
    ImGui::Begin("Spot Light");

    ImGui::DragFloat3("Position", reinterpret_cast<float*>(&_light.position), slider_drag_speed * 0.1f);

    if (ImGui::SliderFloat3("Direction", reinterpret_cast<float*>(&_light.direction), -1.0f, 1.0f))
        _light.direction = glm::normalize(_light.direction);

    float inner_cutoff = glm::acos(_light.inner_cutoff);
    float outer_cutoff = glm::acos(_light.outer_cutoff);

    if (ImGui::SliderAngle("Inner Cutoff", &inner_cutoff, 0.0f, 180.0f))
        _light.inner_cutoff = glm::cos(inner_cutoff);

    if (ImGui::SliderAngle("Outer Cutoff", &outer_cutoff, 0.0f, 180.0f))
        _light.outer_cutoff = glm::cos(outer_cutoff);

    // @cleanup: duplicated code
    ImGui::ColorPicker3("Color", reinterpret_cast<float*>(&_light.properties.color));
    ImGui::DragFloat3("Ambient", reinterpret_cast<float*>(&_light.properties.ambient), slider_drag_speed * 0.1f);
    ImGui::DragFloat3("Diffuse", reinterpret_cast<float*>(&_light.properties.diffuse), slider_drag_speed);
    ImGui::DragFloat3("Specular", reinterpret_cast<float*>(&_light.properties.specular), slider_drag_speed);

    static_assert(sizeof(_light.attenuation) == 3 * sizeof(float));
    ImGui::DragFloat3("Attenuation", reinterpret_cast<float*>(&_light.attenuation), slider_drag_speed * 0.01f);

    ImGui::End();
}

auto ScenePickerUi::on_update() -> void
{
    ImGui::Begin("Scene");

    ImGui::Text("%s", _scene_names[_selected_scene_idx].c_str());

    auto prev_scene_label = ZTH_FORMAT("Prev ({})", prev_scene_key);
    ImGui::TextUnformatted(prev_scene_label.c_str());

    ImGui::SameLine();

    if (ImGui::ArrowButton("Prev", ImGuiDir_Left))
        prev();

    ImGui::SameLine();

    if (ImGui::ArrowButton("Next", ImGuiDir_Right))
        next();

    ImGui::SameLine();

    auto next_scene_label = ZTH_FORMAT("Next ({})", next_scene_key);
    ImGui::TextUnformatted(next_scene_label.c_str());

    ImGui::End();
}

auto ScenePickerUi::on_key_pressed_event(const KeyPressedEvent& event) -> void
{
    if (event.key == prev_scene_key)
        prev();
    else if (event.key == next_scene_key)
        next();
}

auto ScenePickerUi::prev() -> void
{
    if (_scene_count == 0)
        return;

    if (_selected_scene_idx == 0)
        _selected_scene_idx = _scene_count - 1;
    else
        _selected_scene_idx--;

    load_scene(_selected_scene_idx);
}

auto ScenePickerUi::next() -> void
{
    if (_scene_count == 0)
        return;

    if (_selected_scene_idx >= _scene_count - 1)
        _selected_scene_idx = 0;
    else
        _selected_scene_idx++;

    load_scene(_selected_scene_idx);
}

auto ScenePickerUi::load_scene(usize idx) const -> void
{
    auto scene = _scene_constructors[idx]();
    SceneManager::load_scene(std::move(scene));
}

} // namespace zth::debug
