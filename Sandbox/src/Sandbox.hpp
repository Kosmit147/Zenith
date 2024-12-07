#pragma once

#include <Zenith/Zenith.hpp>

class Sandbox : public zth::Application
{
public:
    explicit Sandbox();
    ZTH_NO_COPY_NO_MOVE(Sandbox)
    ~Sandbox() override = default;

private:
    auto on_update() -> void override;
    auto on_event(const zth::Event& event) -> void override;
};
