#include "MainScene.hpp"

#include <battery/embed.hpp>
#include <imgui.h>

#include <optional>

namespace {

const auto cobble_diffuse_map = b::embed<"assets/cobble_diffuse.png">().vec();
const auto container_diffuse_map = b::embed<"assets/container_diffuse.jpg">().vec();
const auto container2_diffuse_map = b::embed<"assets/container2_diffuse.png">().vec();
const auto container2_specular_map = b::embed<"assets/container2_specular.png">().vec();
const auto emoji_diffuse_map = b::embed<"assets/emoji_diffuse.png">().vec();
const auto matrix_emission_map = b::embed<"assets/matrix_emission.jpg">().vec();
const auto wall_diffuse_map = b::embed<"assets/wall_diffuse.jpg">().vec();

const zth::TextureParams cobble_diffuse_map_params = {
    .min_filter = zth::TextureMinFilter::nearest_mipmap_linear,
    .mag_filter = zth::TextureMagFilter::nearest,
};

constexpr auto camera_position = glm::vec3{ 0.0f, 0.0f, 5.0f };
constexpr auto camera_front = glm::vec3{ 0.0f, 0.0f, -1.0f };

constexpr auto aspect_ratio = 16.0f / 9.0f;
constexpr auto fov = glm::radians(45.0f);

constexpr auto light_position = glm::vec3{ -0.7f, 1.3f, 1.7f };
constexpr auto light_color = glm::vec3{ 1.0f };

} // namespace

MainScene::MainScene()
    : _light_marker_material{ .shader = &zth::shaders::flat_color(), .albedo = light_color },
      _camera(std::make_shared<zth::PerspectiveCamera>(camera_position, camera_front, aspect_ratio, fov)),
      _camera_controller(_camera), _light(std::make_shared<zth::PointLight>(light_position, light_color))
{
    _light_marker.translate(_light->translation()).scale(0.1f);

    _diffuse_maps.emplace("Cobble", zth::Texture2D{ cobble_diffuse_map, cobble_diffuse_map_params });
    _diffuse_maps.emplace("Container", zth::Texture2D{ container_diffuse_map });
    _diffuse_maps.emplace("Container2", zth::Texture2D{ container2_diffuse_map });
    _diffuse_maps.emplace("Emoji", zth::Texture2D{ emoji_diffuse_map });
    _diffuse_maps.emplace("Wall", zth::Texture2D{ wall_diffuse_map });

    _specular_maps.emplace("Container2", zth::Texture2D{ container2_specular_map });

    _emission_maps.emplace("Matrix", zth::Texture2D{ matrix_emission_map });
}

auto MainScene::on_load() -> void
{
    zth::Renderer::set_camera(_camera);
    zth::Renderer::set_light(_light);
}

auto MainScene::on_update() -> void
{
    draw_ui();

    if (!zth::Window::cursor_enabled())
        _camera_controller.on_update();

    _rotation_axis = glm::normalize(_rotation_axis);

    if (_uniform_scale)
        _scale = glm::vec3{ _scale.x };

    _cube.set_translation(_translation);
    _cube.set_rotation(_rotation_angle, _rotation_axis);
    _cube.set_scale(_scale);

    _light_marker_material.albedo = _light->color;

    if (_material_was_changed)
    {
        auto& materials = zth::materials::materials();
        ZTH_ASSERT(_material_selected_index < materials.size());
        _material = materials[_material_selected_index];
        _material_was_changed = false;
        _diffuse_map_selected = no_map_selected;
        _specular_map_selected = no_map_selected;
        _emission_map_selected = no_map_selected;
    }

    zth::Renderer::set_wireframe_mode(_wireframe_mode_enabled);
    zth::Renderer::draw(_cube, _material);
    zth::Renderer::draw(_light_marker, _light_marker_material);
}

auto MainScene::on_event(const zth::Event& event) -> void
{
    switch (event.type())
    {
        using enum zth::EventType;
    case WindowResized:
    {
        auto window_resized_event = event.window_resized_event();
        on_window_resized_event(window_resized_event);
    }
    break;
    case KeyPressed:
    {
        auto key_pressed_event = event.key_pressed_event();
        on_key_pressed_event(key_pressed_event);
    }
    break;
    default:
        break;
    }
}

auto MainScene::on_window_resized_event(const zth::WindowResizedEvent& event) const -> void
{
    auto new_size = event.new_size;
    _camera->set_aspect_ratio(static_cast<float>(new_size.x) / static_cast<float>(new_size.y));
}

auto MainScene::on_key_pressed_event(const zth::KeyPressedEvent& event) -> void
{
    if (event.key == _toggle_wireframe_mode_key)
        _wireframe_mode_enabled = !_wireframe_mode_enabled;
}

auto MainScene::draw_ui() -> void
{
    draw_transform_ui();
    draw_light_ui();
    draw_material_ui();
    draw_debug_tools_ui();
}

auto MainScene::draw_transform_ui() -> void
{
    ImGui::Begin("Transform");

    ImGui::DragFloat3("Translation", reinterpret_cast<float*>(&_translation), _ui_slider_drag_speed);
    ImGui::DragFloat("Rotation Angle", &_rotation_angle, _ui_slider_drag_speed);
    ImGui::DragFloat3("Rotation Axis", reinterpret_cast<float*>(&_rotation_axis), _ui_slider_drag_speed);

    if (_uniform_scale)
        ImGui::DragFloat("Scale", reinterpret_cast<float*>(&_scale), _ui_slider_drag_speed);
    else
        ImGui::DragFloat3("Scale", reinterpret_cast<float*>(&_scale), _ui_slider_drag_speed);

    ImGui::Checkbox("Uniform Scale", &_uniform_scale);

    ImGui::End();
}

auto MainScene::draw_light_ui() -> void
{
    ImGui::Begin("Light");

    ImGui::ColorPicker3("Color", reinterpret_cast<float*>(&_light->color));
    ImGui::DragFloat3("Ambient", reinterpret_cast<float*>(&_light->ambient), _ui_slider_drag_speed * 0.1f);
    ImGui::DragFloat3("Diffuse", reinterpret_cast<float*>(&_light->diffuse), _ui_slider_drag_speed);
    ImGui::DragFloat3("Specular", reinterpret_cast<float*>(&_light->specular), _ui_slider_drag_speed);

    ImGui::End();
}

auto MainScene::draw_material_ui() -> void
{
    ImGui::Begin("Material");

    constexpr std::array material_names = {
        "plain",         "emerald",      "jade",          "obsidian",       "pearl",
        "ruby",          "turquoise",    "brass",         "bronze",         "chrome",
        "copper",        "gold",         "silver",        "black_plastic",  "cyan_plastic",
        "green_plastic", "red_plastic",  "white_plastic", "yellow_plastic", "black_rubber",
        "cyan_rubber",   "green_rubber", "red_rubber",    "white_rubber",   "yellow_rubber",
    };

    ZTH_ASSERT(material_names.size() == zth::materials::materials().size());

    if (ImGui::BeginCombo("Preset", material_names[_material_selected_index]))
    {
        for (std::size_t n = 0; n < material_names.size(); n++)
        {
            const auto is_selected = _material_selected_index == n;

            if (ImGui::Selectable(material_names[n], is_selected))
            {
                _material_selected_index = n;
                _material_was_changed = true;
            }

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }

        ImGui::EndCombo();
    }

    ImGui::Spacing();

    ImGui::ColorPicker3("Albedo", reinterpret_cast<float*>(&_material.albedo));

    ImGui::Spacing();

    auto map_picker = [](std::string_view label, std::string& selected,
                         const zth::StringMap<zth::Texture2D>& texture_map) {
        std::optional<const zth::Texture2D*> pick = std::nullopt;

        if (ImGui::BeginCombo(label.data(), selected.c_str()))
        {
            for (const auto& [name, texture] : texture_map)
            {
                const auto is_selected = selected == name;

                if (ImGui::Selectable(name.c_str(), is_selected))
                {
                    selected = name;
                    pick = &texture;
                }

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }

            {
                constexpr auto name = no_map_selected.data();
                const auto is_selected = selected == name;

                if (ImGui::Selectable(name, is_selected))
                {
                    selected = name;
                    pick = nullptr;
                }

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }

        return pick;
    };

    if (auto pick = map_picker("Diffuse Map", _diffuse_map_selected, _diffuse_maps))
        _material.diffuse_map = *pick;

    if (auto pick = map_picker("Specular Map", _specular_map_selected, _specular_maps))
        _material.specular_map = *pick;

    if (auto pick = map_picker("Emission Map", _emission_map_selected, _emission_maps))
        _material.emission_map = *pick;

    ImGui::DragFloat3("Ambient", reinterpret_cast<float*>(&_material.ambient), _ui_slider_drag_speed * 0.1f);
    ImGui::DragFloat3("Diffuse", reinterpret_cast<float*>(&_material.diffuse), _ui_slider_drag_speed);
    ImGui::DragFloat3("Specular", reinterpret_cast<float*>(&_material.specular), _ui_slider_drag_speed);
    ImGui::DragFloat("Shininess", &_material.shininess, _ui_slider_drag_speed * 20.0f);

    ImGui::End();
}

auto MainScene::draw_debug_tools_ui() -> void
{
    ImGui::Begin("Debug Tools");

    auto label = fmt::format("Wireframe ({})", _toggle_wireframe_mode_key);
    ImGui::Checkbox(label.c_str(), &_wireframe_mode_enabled);

    ImGui::End();
}
