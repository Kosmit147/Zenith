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
    zth::VertexBuffer<Vertex> _vb;
    zth::IndexBuffer<GLushort> _ib;
    zth::Shader _shader;
    zth::Texture2D _texture;

private:
    auto on_update() -> void override;
};
