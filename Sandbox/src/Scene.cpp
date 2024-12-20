#include "Scene.hpp"

#include <battery/embed.hpp>

namespace {

const auto container_texture = b::embed<"assets/container.jpg">().vec();
const auto emoji_texture = b::embed<"assets/emoji.png">().vec();
const auto wall_texture = b::embed<"assets/wall.jpg">().vec();

constexpr auto camera_position = glm::vec3(0.0f, 0.0f, 5.0f);
constexpr auto camera_front = glm::vec3(0.0f, 0.0f, -1.0f);

constexpr auto aspect_ratio = 16.0f / 9.0f;
constexpr auto fov = glm::radians(45.0f);

constexpr auto light_position = glm::vec3{ 0.0f, 5.0f, 5.0f };
constexpr auto light_color = glm::vec3{ 1.0f };

} // namespace

Scene::Scene()
    : _cube_texture(wall_texture), _cube_material{ .texture = &_cube_texture },
      _camera(std::make_shared<zth::PerspectiveCamera>(camera_position, camera_front, aspect_ratio, fov)),
      _camera_controller(_camera), _light(std::make_shared<zth::PointLight>(light_position, light_color))
{}

auto Scene::on_load() -> void
{
    zth::Renderer::set_camera(_camera);
    zth::Renderer::set_light(_light);
}

auto Scene::on_update() -> void
{
    const auto time = zth::Time::time<float>();

    if (!zth::Window::cursor_enabled())
        _camera_controller.on_update();

    _cube.rotate(0.0005f * time, glm::normalize(glm::vec3{ 0.0f, 1.0f, 0.0f }));
    _cube.rotate(0.0005f * time, glm::normalize(glm::vec3{ -0.3f, 0.1f, 0.7f }));

    zth::Renderer::draw(_cube, _cube_material);
}

auto Scene::on_event(const zth::Event& event) -> void
{
    if (event.type() == zth::EventType::WindowResized)
    {
        auto window_resized_event = event.window_resized_event();
        on_window_resized_event(window_resized_event);
    }
}

auto Scene::on_window_resized_event(const zth::WindowResizedEvent& event) const -> void
{
    auto new_size = event.new_size;
    _camera->set_aspect_ratio(static_cast<float>(new_size.x) / static_cast<float>(new_size.y));
}
