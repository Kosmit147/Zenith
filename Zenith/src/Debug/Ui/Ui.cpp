#include "Zenith/Debug/Ui/Ui.hpp"

#include <glm/geometric.hpp>
#include <imgui.h>
#include <spdlog/spdlog.h>

#include "Zenith/Core/Transformable.hpp"
#include "Zenith/Graphics/Light.hpp"
#include "Zenith/Graphics/Renderer.hpp"
#include "Zenith/Platform/Events.hpp"

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

    auto label = fmt::format("Wireframe ({})", toggle_wireframe_mode_key);

    if (ImGui::Checkbox(label.c_str(), &_wireframe_mode_enabled))
        Renderer::set_wireframe_mode(_wireframe_mode_enabled);

    ImGui::End();
}

TransformUi::TransformUi(Transformable3D& transformable) : _transformable(transformable) {}

auto TransformUi::on_update() -> void
{
    ImGui::Begin("Transform");

    if (ImGui::DragFloat3("Translation", reinterpret_cast<float*>(&_translation), slider_drag_speed))
        _transformable.set_translation(_translation);

    if (ImGui::DragFloat("Rotation Angle", &_rotation_angle, slider_drag_speed))
        _transformable.set_rotation(_rotation_angle, _rotation_axis);

    if (ImGui::DragFloat3("Rotation Axis", reinterpret_cast<float*>(&_rotation_axis), slider_drag_speed))
    {
        _rotation_axis = glm::normalize(_rotation_axis);
        _transformable.set_rotation(_rotation_angle, _rotation_axis);
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

DirectionalLightUi::DirectionalLightUi(DirectionalLight& light) : _light(light), _light_direction(_light.direction()) {}

auto DirectionalLightUi::on_update() -> void
{
    _light_direction = _light.direction();

    ImGui::Begin("Directional Light");

    if (ImGui::DragFloat3("Direction", reinterpret_cast<float*>(&_light_direction), slider_drag_speed))
        _light.set_direction(glm::normalize(_light_direction));

    ImGui::ColorPicker3("Color", reinterpret_cast<float*>(&_light.properties.color));
    ImGui::DragFloat3("Ambient", reinterpret_cast<float*>(&_light.properties.ambient), slider_drag_speed * 0.1f);
    ImGui::DragFloat3("Diffuse", reinterpret_cast<float*>(&_light.properties.diffuse), slider_drag_speed);
    ImGui::DragFloat3("Specular", reinterpret_cast<float*>(&_light.properties.specular), slider_drag_speed);

    ImGui::End();
}

PointLightUi::PointLightUi(PointLight& light) : _light(light), _light_position(_light.translation()) {}

auto PointLightUi::on_update() -> void
{
    _light_position = _light.translation();

    ImGui::Begin("Point Light");

    if (ImGui::DragFloat3("Position", reinterpret_cast<float*>(&_light_position), slider_drag_speed * 0.1f))
        _light.set_translation(_light_position);

    ImGui::ColorPicker3("Color", reinterpret_cast<float*>(&_light.properties.color));
    ImGui::DragFloat3("Ambient", reinterpret_cast<float*>(&_light.properties.ambient), slider_drag_speed * 0.1f);
    ImGui::DragFloat3("Diffuse", reinterpret_cast<float*>(&_light.properties.diffuse), slider_drag_speed);
    ImGui::DragFloat3("Specular", reinterpret_cast<float*>(&_light.properties.specular), slider_drag_speed);

    ImGui::End();
}

} // namespace zth::debug
