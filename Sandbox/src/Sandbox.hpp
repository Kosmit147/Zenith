#pragma once

#include <Zenith/Zenith.hpp>

class Sandbox : public zth::Application
{
public:
    explicit Sandbox();
    ~Sandbox();

private:
    GLuint _shader;

private:
    auto on_update() -> void override;
};
