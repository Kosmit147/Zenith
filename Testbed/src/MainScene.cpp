#include "MainScene.hpp"

#include <imgui.h>

#include <optional>

#include "Embedded.hpp"
#include "Testbed.hpp"

namespace {

const zth::TextureParams cobble_diffuse_map_params = {
    .min_filter = zth::TextureMinFilter::nearest_mipmap_linear,
    .mag_filter = zth::TextureMagFilter::nearest,
};

constexpr auto camera_position = glm::vec3{ 0.0f, 0.0f, 5.0f };
constexpr auto camera_front = glm::vec3{ 0.0f, 0.0f, -1.0f };

constexpr auto aspect_ratio = 16.0f / 9.0f;
constexpr auto fov = glm::radians(45.0f);

const auto directional_light_direction = glm::normalize(glm::vec3{ 0.0f, -1.0f, 0.0f });
constexpr auto directional_light_properties = zth::LightProperties{ .color = glm::vec3{ 0.0f } };

constexpr auto point_light_position = glm::vec3{ -0.7f, 1.3f, 1.7f };
constexpr auto point_light_properties = zth::LightProperties{};

} // namespace

MainScene::MainScene()
    : _light_marker_material{ .shader = &zth::shaders::flat_color(), .albedo = point_light_properties.color },
      _camera(std::make_shared<zth::PerspectiveCamera>(camera_position, camera_front, aspect_ratio, fov)),
      _camera_controller(_camera), _directional_light(std::make_shared<zth::DirectionalLight>(
                                       directional_light_direction, directional_light_properties)),
      _point_light(std::make_shared<zth::PointLight>(point_light_position, point_light_properties))
{
    _transform_gizmo.toggle_key = Testbed::toggle_cursor_key;
    _light_marker.set_translation(_point_light->position).set_scale(0.1f);

    _diffuse_maps.emplace("Cobble", zth::Texture2D{ embedded::cobble_diffuse_map_data, cobble_diffuse_map_params });
    _diffuse_maps.emplace("Container", zth::Texture2D{ embedded::container_diffuse_map_data });
    _diffuse_maps.emplace("Container2", zth::Texture2D{ embedded::container2_diffuse_map_data });
    _diffuse_maps.emplace("Emoji", zth::Texture2D{ embedded::emoji_diffuse_map_data });
    _diffuse_maps.emplace("Wall", zth::Texture2D{ embedded::wall_diffuse_map_data });

    _specular_maps.emplace("Container2", zth::Texture2D{ embedded::container2_specular_map_data });

    _emission_maps.emplace("Matrix", zth::Texture2D{ embedded::matrix_emission_map_data });
}

auto MainScene::on_load() -> void
{
    _transform_gizmo.enabled = zth::Window::cursor_enabled();

    zth::Renderer::set_camera(_camera);
    zth::Renderer::set_directional_light(_directional_light);
    zth::Renderer::set_point_light(_point_light);
}

auto MainScene::on_update() -> void
{
    update_ui();

    if (!zth::Window::cursor_enabled())
        _camera_controller.on_update();

    _light_marker.set_translation(_point_light->position);
    _light_marker_material.albedo = _point_light->properties.color;

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

auto MainScene::on_window_resized_event(const zth::WindowResizedEvent& event) -> void
{
    auto new_size = event.new_size;
    _camera->set_aspect_ratio(static_cast<float>(new_size.x) / static_cast<float>(new_size.y));
}

auto MainScene::on_key_pressed_event(const zth::KeyPressedEvent& event) -> void
{
    _transform_gizmo.on_key_pressed_event(event);
}

auto MainScene::update_ui() -> void
{
    _transform_ui.on_update();
    _transform_gizmo.on_update();
    _directional_light_ui.on_update();
    _point_light_ui.on_update();
    draw_material_ui();
}

auto MainScene::draw_material_ui() -> void
{
    // TODO: move into Zenith debug uis

    constexpr auto ui_slider_drag_speed = 0.01f;

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

    ImGui::DragFloat3("Ambient", reinterpret_cast<float*>(&_material.ambient), ui_slider_drag_speed * 0.1f);
    ImGui::DragFloat3("Diffuse", reinterpret_cast<float*>(&_material.diffuse), ui_slider_drag_speed);
    ImGui::DragFloat3("Specular", reinterpret_cast<float*>(&_material.specular), ui_slider_drag_speed);
    ImGui::DragFloat("Shininess", &_material.shininess, ui_slider_drag_speed * 20.0f);

    ImGui::End();
}
