#include "TransformTest.hpp"

#include <battery/embed.hpp>
#include <imgui.h>

namespace {

const auto container_texture = b::embed<"assets/container.jpg">().vec();
const auto emoji_texture = b::embed<"assets/emoji.png">().vec();
const auto wall_texture = b::embed<"assets/wall.jpg">().vec();
const auto cobble_texture = b::embed<"assets/cobble.png">().vec();

const zth::TextureParams cobble_texture_params = {
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

TransformTest::TransformTest()
    : _container_texture(container_texture), _emoji_texture(emoji_texture), _wall_texture(wall_texture),
      _cobble_texture(cobble_texture, cobble_texture_params),
      _light_cube_material{ .shader = &zth::shaders::flat_color(), .albedo = light_color },
      _camera(std::make_shared<zth::PerspectiveCamera>(camera_position, camera_front, aspect_ratio, fov)),
      _camera_controller(_camera), _light(std::make_shared<zth::PointLight>(light_position, light_color))
{
    _light_marker.translate(_light->translation()).scale(0.1f);
}

auto TransformTest::on_load() -> void
{
    zth::Renderer::set_camera(_camera);
    zth::Renderer::set_light(_light);
}

auto TransformTest::on_update() -> void
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

    _light_cube_material.albedo = _light->color;

    if (_material_was_changed)
    {
        auto& materials = zth::materials::materials();
        ZTH_ASSERT(_material_selected_index < materials.size());
        _cube_material = materials[_material_selected_index];
        _material_was_changed = false;
        _tex_selected_index = 4;
    }

    if (_tex_was_changed)
    {
        switch (_tex_selected_index)
        {
        case 0:
            _cube_material.texture = &_container_texture;
            break;
        case 1:
            _cube_material.texture = &_emoji_texture;
            break;
        case 2:
            _cube_material.texture = &_wall_texture;
            break;
        case 3:
            _cube_material.texture = &_cobble_texture;
            break;
        case 4:
            _cube_material.texture = nullptr;
            break;
        default:
            ZTH_ASSERT(false);
            break;
        }

        _tex_was_changed = false;
    }

    zth::Renderer::draw(_cube, _cube_material);
    zth::Renderer::draw(_light_marker, _light_cube_material);
}

auto TransformTest::on_event(const zth::Event& event) -> void
{
    if (event.type() == zth::EventType::WindowResized)
    {
        auto window_resized_event = event.window_resized_event();
        on_window_resized_event(window_resized_event);
    }
}

auto TransformTest::on_window_resized_event(const zth::WindowResizedEvent& event) const -> void
{
    auto new_size = event.new_size;
    _camera->set_aspect_ratio(static_cast<float>(new_size.x) / static_cast<float>(new_size.y));
}

auto TransformTest::draw_ui() -> void
{
    draw_transform_ui();
    draw_light_ui();
    draw_material_ui();
}

auto TransformTest::draw_transform_ui() -> void
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

auto TransformTest::draw_light_ui() -> void
{
    ImGui::Begin("Light");

    ImGui::ColorPicker3("Color", reinterpret_cast<float*>(&_light->color));
    ImGui::DragFloat3("Ambient", reinterpret_cast<float*>(&_light->ambient), _ui_slider_drag_speed * 0.1f);
    ImGui::DragFloat3("Diffuse", reinterpret_cast<float*>(&_light->diffuse), _ui_slider_drag_speed);
    ImGui::DragFloat3("Specular", reinterpret_cast<float*>(&_light->specular), _ui_slider_drag_speed);

    ImGui::End();
}

auto TransformTest::draw_material_ui() -> void
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

    ImGui::ColorPicker3("Albedo", reinterpret_cast<float*>(&_cube_material.albedo));

    ImGui::Spacing();

    constexpr std::array textures = { "Container", "Emoji", "Wall", "Cobble", "None" };

    if (ImGui::BeginCombo("Texture", textures[_tex_selected_index]))
    {
        for (std::size_t n = 0; n < textures.size(); n++)
        {
            const auto is_selected = _tex_selected_index == n;

            if (ImGui::Selectable(textures[n], is_selected))
            {
                _tex_selected_index = n;
                _tex_was_changed = true;
            }

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }

        ImGui::EndCombo();
    }

    ImGui::DragFloat3("Ambient", reinterpret_cast<float*>(&_cube_material.ambient), _ui_slider_drag_speed * 0.1f);
    ImGui::DragFloat3("Diffuse", reinterpret_cast<float*>(&_cube_material.diffuse), _ui_slider_drag_speed);
    ImGui::DragFloat3("Specular", reinterpret_cast<float*>(&_cube_material.specular), _ui_slider_drag_speed);
    ImGui::DragFloat("Shininess", &_cube_material.shininess, _ui_slider_drag_speed * 20.0f);

    ImGui::End();
}
