#include "Zenith/Debug/Ui/Ui.hpp"

#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>

#include "Zenith/Core/SceneManager.hpp"
#include "Zenith/Core/Transformable.hpp"
#include "Zenith/Graphics/Light.hpp"
#include "Zenith/Graphics/Material.hpp"
#include "Zenith/Graphics/Materials.hpp"
#include "Zenith/Graphics/Renderer.hpp"
#include "Zenith/Platform/Events.hpp"
#include "Zenith/Platform/Window.hpp"

namespace zth::debug {

namespace {

constexpr auto slider_drag_speed = 0.01f;

} // namespace

auto DebugToolsUi::on_key_pressed_event(const KeyPressedEvent& event) -> void
{
    if (event.key == toggle_wireframe_mode_key)
    {
        _wireframe_mode_enabled = !_wireframe_mode_enabled;
        Renderer::set_wireframe_mode(_wireframe_mode_enabled);
    }
}

auto DebugToolsUi::on_update() -> void
{
    ImGui::Begin("Debug Tools");

    auto fps = ImGui::GetIO().Framerate;
    ImGui::Text("FPS: %0.0f", fps);

    {
        auto limit = Window::frame_rate_limit();
        _frame_rate_limit = limit.value_or(60);
        _frame_rate_limit_enabled = limit.has_value();
    }

    if (ImGui::Checkbox("FPS Limit", &_frame_rate_limit_enabled))
    {
        if (_frame_rate_limit_enabled)
            Window::set_frame_rate_limit(_frame_rate_limit);
        else
            Window::disable_frame_rate_limit();
    }

    if (_frame_rate_limit_enabled)
    {
        int limit = static_cast<int>(_frame_rate_limit);

        if (ImGui::InputInt("##", &limit))
        {
            _frame_rate_limit = static_cast<u32>(limit);
            Window::set_frame_rate_limit(_frame_rate_limit);
        }
    }

    auto label = fmt::format("Wireframe ({})", toggle_wireframe_mode_key);

    if (ImGui::Checkbox(label.c_str(), &_wireframe_mode_enabled))
        Renderer::set_wireframe_mode(_wireframe_mode_enabled);

    ImGui::End();
}

TransformUi::TransformUi(Transformable3D& transformable) : _transformable(transformable) {}

auto TransformUi::on_update() -> void
{
    _translation = _transformable.translation();
    _scale = _transformable.scale();

    ImGui::Begin("Transform");

    if (ImGui::DragFloat3("Translation", reinterpret_cast<float*>(&_translation), slider_drag_speed))
        _transformable.set_translation(_translation);

    if (ImGui::DragFloat("Rotation Angle", &_rotation.angle, slider_drag_speed))
        _transformable.set_rotation(_rotation);

    if (ImGui::DragFloat3("Rotation Axis", reinterpret_cast<float*>(&_rotation.axis), slider_drag_speed))
    {
        _rotation.axis = glm::normalize(_rotation.axis);
        _transformable.set_rotation(_rotation);
    }

    if (_uniform_scale)
    {
        if (ImGui::DragFloat("Scale", reinterpret_cast<float*>(&_scale), slider_drag_speed))
        {
            _scale = glm::vec3{ _scale.x };
            _transformable.set_scale(_scale);
        }
    }
    else
    {
        if (ImGui::DragFloat3("Scale", reinterpret_cast<float*>(&_scale), slider_drag_speed))
        {
            _transformable.set_scale(_scale);
        }
    }

    if (ImGui::Checkbox("Uniform Scale", &_uniform_scale))
    {
        if (_uniform_scale)
        {
            _scale = glm::vec3{ _scale.x };
            _transformable.set_scale(_scale);
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
        for (const auto [i, name] : material_names | std::views::enumerate)
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

    auto map_picker = [](std::string_view label, i64 selected_idx, const std::vector<std::string>& map_names) {
        constexpr auto none_selected_label = "None";
        std::optional<i64> pick = std::nullopt;

        std::string_view selected_map_name = none_selected_label;

        if (selected_idx >= 0)
            selected_map_name = map_names[selected_idx];

        if (ImGui::BeginCombo(label.data(), selected_map_name.data()))
        {
            {
                const auto is_selected = selected_idx == _none_selected;

                if (ImGui::Selectable(none_selected_label, is_selected))
                    pick = _none_selected;

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }

            for (const auto [i, map] : map_names | std::views::enumerate)
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
    ImGui::DragFloat("Shininess", &_material.shininess, slider_drag_speed * 20.0f);

    ImGui::End();
}

auto MaterialUi::add_diffuse_map(std::string_view name, const Texture2D& diffuse_map) -> void
{
    _diffuse_map_names.emplace_back(name);
    _diffuse_maps.push_back(&diffuse_map);
}

auto MaterialUi::add_specular_map(std::string_view name, const Texture2D& specular_map) -> void
{
    _specular_map_names.emplace_back(name);
    _specular_maps.push_back(&specular_map);
}

auto MaterialUi::add_emission_map(std::string_view name, const Texture2D& emission_map) -> void
{
    _emission_map_names.emplace_back(name);
    _emission_maps.push_back(&emission_map);
}

auto MaterialUi::set_diffuse_map(i64 idx) -> void
{
    _diffuse_map_selected_idx = idx;
    _material.diffuse_map = idx >= 0 ? _diffuse_maps[idx] : nullptr;
}

auto MaterialUi::set_specular_map(i64 idx) -> void
{
    _specular_map_selected_idx = idx;
    _material.specular_map = idx >= 0 ? _specular_maps[idx] : nullptr;
}

auto MaterialUi::set_emission_map(i64 idx) -> void
{
    _emission_map_selected_idx = idx;
    _material.emission_map = idx >= 0 ? _emission_maps[idx] : nullptr;
}

DirectionalLightUi::DirectionalLightUi(DirectionalLight& light) : _light(light) {}

auto DirectionalLightUi::on_update() -> void
{
    ImGui::Begin("Directional Light");

    if (ImGui::DragFloat3("Direction", reinterpret_cast<float*>(&_light.direction), slider_drag_speed))
        _light.direction = glm::normalize(_light.direction);

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

    ImGui::DragFloat3("Position", reinterpret_cast<float*>(&_light.position), slider_drag_speed * 0.1f);
    ImGui::ColorPicker3("Color", reinterpret_cast<float*>(&_light.properties.color));
    ImGui::DragFloat3("Ambient", reinterpret_cast<float*>(&_light.properties.ambient), slider_drag_speed * 0.1f);
    ImGui::DragFloat3("Diffuse", reinterpret_cast<float*>(&_light.properties.diffuse), slider_drag_speed);
    ImGui::DragFloat3("Specular", reinterpret_cast<float*>(&_light.properties.specular), slider_drag_speed);

    static_assert(sizeof(_light.attenuation) == 3 * sizeof(float));
    ImGui::DragFloat3("Attenuation", reinterpret_cast<float*>(&_light.attenuation), slider_drag_speed * 0.1f);

    ImGui::End();
}

auto ScenePickerUi::on_update() -> void
{
    ImGui::Begin("Scene");

    ImGui::Text("%s", _scene_names[_selected_scene_idx].c_str());

    auto prev_scene_label = fmt::format("Prev ({})", prev_scene_key);
    ImGui::TextUnformatted(prev_scene_label.c_str());

    ImGui::SameLine();

    if (ImGui::ArrowButton("Prev", ImGuiDir_Left))
        prev();

    ImGui::SameLine();

    if (ImGui::ArrowButton("Next", ImGuiDir_Right))
        next();

    ImGui::SameLine();

    auto next_scene_label = fmt::format("Next ({})", next_scene_key);
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

auto ScenePickerUi::load_scene(usize idx) -> void
{
    auto scene = _scene_constructors[idx]();
    SceneManager::load_scene(std::move(scene));
}

} // namespace zth::debug
