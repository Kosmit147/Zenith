#pragma once

#include <Zenith/Zenith.hpp>
#include <glm/glm.hpp>

struct Vertex
{
    glm::vec4 color;
    glm::vec2 tex_coords;
};

class Sandbox : public zth::Application
{
public:
    explicit Sandbox();
    ~Sandbox() override = default;

private:
    zth::VertexArray _va;
    zth::VertexBuffer _vb;
    zth::IndexBuffer _ib;
    zth::Shader _shader;
    zth::Texture2D _texture;
    zth::PerspectiveCamera _camera;

private:
    auto on_update() -> void override;
    auto on_event(const zth::Event& event) -> void override;

    auto on_window_resized_event(const zth::WindowResizedEvent& event) -> void;
    auto update_camera() -> void;
};
