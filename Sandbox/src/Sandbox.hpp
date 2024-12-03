#pragma once

#include <Zenith/Zenith.hpp>
#include <glm/glm.hpp>

#include <memory>

struct Vertex
{
    glm::vec3 position;
    glm::vec2 tex_coords;
    glm::vec4 color;
};

class Sandbox : public zth::Application
{
public:
    explicit Sandbox();
    ~Sandbox() override = default;

private:
    zth::Mesh _mesh;
    zth::Shader _shader;
    zth::Texture2D _texture;
    std::shared_ptr<zth::PerspectiveCamera> _camera;

private:
    auto on_update() -> void override;
    auto on_event(const zth::Event& event) -> void override;

    auto on_window_resized_event(const zth::WindowResizedEvent& event) const -> void;
    auto update_camera() const -> void;
};
