#pragma once

#include <Zenith/Zenith.hpp>

class Sandbox : public zth::Application
{
public:
    explicit Sandbox();
    ~Sandbox() override = default;

private:
    zth::VertexArray _va;
    zth::IndexBuffer _ib;
    zth::Shader _shader;

private:
    auto on_update() -> void override;
};
