#pragma once

#include <Zenith/Zenith.hpp>

class Sandbox : public zth::Application
{
public:
    explicit Sandbox();

private:
    auto on_update() -> void override;
};
