#pragma once

#include <Zenith/Zenith.hpp>

class MainScene : public zth::Scene
{
public:
    explicit MainScene();
    ZTH_NO_COPY_NO_MOVE(MainScene)
    ~MainScene() override = default;

private:
    zth::CubeShape _cube;
    zth::SphereShape _light_marker;

    zth::Texture2D _cobble_diffuse_map;
    zth::Texture2D _container_diffuse_map;
    zth::Texture2D _container2_diffuse_map;
    zth::Texture2D _emoji_diffuse_map;
    zth::Texture2D _wall_diffuse_map;

    zth::Texture2D _container2_specular_map;

    zth::Material _cube_material;
    zth::Material _light_cube_material;

    std::shared_ptr<zth::PerspectiveCamera> _camera;
    zth::FpsCameraController _camera_controller;
    std::shared_ptr<zth::PointLight> _light;

    glm::vec3 _translation = glm::vec3{ 0.0f };
    float _rotation_angle = 0.0f;
    glm::vec3 _rotation_axis = glm::vec3{ 0.0f, 1.0f, 0.0f };
    glm::vec3 _scale = glm::vec3{ 1.0f };
    bool _uniform_scale = true;

    std::size_t _material_selected_index = 0;
    bool _material_was_changed = true;

    std::size_t _diffuse_map_selected_index = 5;
    bool _diffuse_map_was_changed = true;

    std::size_t _specular_map_selected_index = 1;
    bool _specular_map_was_changed = true;

    bool _wireframe_mode_enabled = false;

    static constexpr auto _ui_slider_drag_speed = 0.01f;
    static constexpr auto _toggle_wireframe_mode_key = zth::Key::F1;

private:
    auto on_load() -> void override;
    auto on_update() -> void override;
    auto on_event(const zth::Event& event) -> void override;

    auto on_window_resized_event(const zth::WindowResizedEvent& event) const -> void;
    auto on_key_pressed_event(const zth::KeyPressedEvent& event) -> void;

    auto draw_ui() -> void;
    auto draw_transform_ui() -> void;
    auto draw_light_ui() -> void;
    auto draw_material_ui() -> void;
    auto draw_debug_tools_ui() -> void;
};
