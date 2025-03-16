#include "main_scene.hpp"

#include "embedded.hpp"
#include "testbed.hpp"

namespace {

constexpr auto camera_position = glm::vec3{ 0.0f, 0.0f, 5.0f };
constexpr auto camera_forward = glm::vec3{ 0.0f, 0.0f, -1.0f };

constexpr auto aspect_ratio = 16.0f / 9.0f;
constexpr auto fov = glm::radians(45.0f);

const auto directional_light = zth::DirectionalLight{
    .direction = glm::normalize(glm::vec3{ 0.0f, -1.0f, 0.0f }),
    .properties = zth::LightProperties{ .color = zth::colors::black },
};

constexpr auto point_light = zth::PointLight{ .position = glm::vec3{ -0.7f, 1.3f, 1.7f },
                                              .attenuation = {
                                                  .linear = 0.09f,
                                                  .quadratic = 0.032f,
                                              } };

constexpr auto point_light2 = zth::PointLight{ .position = glm::vec3{ -0.2f, 2.3f, 0.1f },
                                               .attenuation = {
                                                   .linear = 0.09f,
                                                   .quadratic = 0.032f,
                                               } };

constexpr auto point_light3 = zth::PointLight{ .position = glm::vec3{ -1.7f, 0.0f, 2.0f },
                                               .attenuation = {
                                                   .linear = 0.09f,
                                                   .quadratic = 0.032f,
                                               } };

const auto cobble_diffuse_map_params = zth::gl::TextureParams{
    .min_filter = zth::gl::TextureMinFilter::nearest_mipmap_linear,
    .mag_filter = zth::gl::TextureMagFilter::nearest,
};

} // namespace

MainScene::MainScene()
    : _light_marker_material{ .shader = &zth::shaders::flat_color(), .albedo = point_light.properties.color },
      _light_marker_material2{ .shader = &zth::shaders::flat_color(), .albedo = point_light.properties.color },
      _light_marker_material3{ .shader = &zth::shaders::flat_color(), .albedo = point_light.properties.color },
      _camera(std::make_shared<zth::PerspectiveCamera>(camera_position, camera_forward, aspect_ratio, fov)),
      _camera_controller(_camera), _directional_light(std::make_shared<zth::DirectionalLight>(directional_light)),
      _point_light(std::make_shared<zth::PointLight>(point_light)),
      _point_light2(std::make_shared<zth::PointLight>(point_light2)),
      _point_light3(std::make_shared<zth::PointLight>(point_light3))
{
    _transform_gizmo.toggle_key = Testbed::toggle_cursor_key;
    _light_marker.set_translation(_point_light->position).set_scale(0.1f);
    _light_marker2.set_translation(_point_light2->position).set_scale(0.1f);
    _light_marker3.set_translation(_point_light3->position).set_scale(0.1f);

    _diffuse_maps.emplace_back(embedded::cobble_diffuse_map_data, cobble_diffuse_map_params);
    _diffuse_maps.emplace_back(embedded::container_diffuse_map_data);
    _diffuse_maps.emplace_back(embedded::container2_diffuse_map_data);
    _diffuse_maps.emplace_back(embedded::emoji_diffuse_map_data);
    _diffuse_maps.emplace_back(embedded::wall_diffuse_map_data);

    std::size_t i = 0;
    _material_ui.add_diffuse_map("Cobble", _diffuse_maps[i++]);
    _material_ui.add_diffuse_map("Container", _diffuse_maps[i++]);
    _material_ui.add_diffuse_map("Container2", _diffuse_maps[i++]);
    _material_ui.add_diffuse_map("Emoji", _diffuse_maps[i++]);
    _material_ui.add_diffuse_map("Wall", _diffuse_maps[i++]);

    _specular_maps.emplace_back(embedded::container2_specular_map_data);
    _material_ui.add_specular_map("Container2", _specular_maps[0]);

    _emission_maps.emplace_back(embedded::matrix_emission_map_data);
    _material_ui.add_emission_map("Matrix", _emission_maps[0]);
}

auto MainScene::on_load() -> void
{
    _transform_gizmo.enabled = zth::Window::cursor_enabled();

    zth::Renderer::set_camera(_camera);
    zth::Renderer::add_directional_light(_directional_light);
    zth::Renderer::add_point_light(_point_light);
    zth::Renderer::add_point_light(_point_light2);
    zth::Renderer::add_point_light(_point_light3);
}

auto MainScene::on_update() -> void
{
    update_ui();

    if (!zth::Window::cursor_enabled())
        _camera_controller.on_update();

    _light_marker.set_translation(_point_light->position);
    _light_marker2.set_translation(_point_light2->position);
    _light_marker3.set_translation(_point_light3->position);
    _light_marker_material.albedo = _point_light->properties.color;
    _light_marker_material2.albedo = _point_light2->properties.color;
    _light_marker_material3.albedo = _point_light3->properties.color;

    zth::Renderer::draw(_cube, _material);
    zth::Renderer::draw(_light_marker, _light_marker_material);
    zth::Renderer::draw(_light_marker2, _light_marker_material2);
    zth::Renderer::draw(_light_marker3, _light_marker_material3);
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
    auto new_width = event.new_size.x;
    auto new_height = event.new_size.y;
    _camera->set_aspect_ratio(static_cast<float>(new_width) / static_cast<float>(new_height));
}

auto MainScene::on_key_pressed_event(const zth::KeyPressedEvent& event) -> void
{
    _transform_gizmo.on_key_pressed_event(event);
}

auto MainScene::update_ui() -> void
{
    _transform_ui.on_update();
    _transform_gizmo.on_update();
    _material_ui.on_update();
    _directional_light_ui.on_update();
    _point_light_ui.on_update();
    _point_light_ui2.on_update();
    _point_light_ui3.on_update();
}
