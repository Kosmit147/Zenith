#include "Zenith/Debug/Ui/Ui.hpp"

#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp>
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

    ImGui::End();
}

} // namespace zth::debug
