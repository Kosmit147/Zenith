#pragma once

#include <Zenith/Zenith.hpp>

class TransformTest : public zth::Scene
{
public:
    explicit TransformTest();
    ZTH_NO_COPY_NO_MOVE(TransformTest)
    ~TransformTest() override = default;

private:
    zth::CubeShape _cube;
    zth::Texture2D _cube_texture;
    zth::Material _cube_material;
    std::shared_ptr<zth::PerspectiveCamera> _camera;

    glm::vec3 _translation = glm::vec3{ 0.0f, 0.0f, 0.0f };
    float _rotation_angle = 0.0f;
    glm::vec3 _rotation_axis = glm::vec3{ 0.0f, 1.0f, 0.0f };
    glm::vec3 _scale = glm::vec3{ 1.0f, 1.0f, 1.0f };
    bool _uniform_scale = true;

private:
    auto on_load() -> void override;
    auto on_update() -> void override;
    auto on_event(const zth::Event& event) -> void override;

    auto on_window_resized_event(const zth::WindowResizedEvent& event) const -> void;

    auto update_camera() const -> void;
    auto render_ui() -> void;
};
