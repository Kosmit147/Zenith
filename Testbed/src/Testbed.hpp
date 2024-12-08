#pragma once

#include <Zenith/Zenith.hpp>

class Testbed : public zth::Application
{
public:
    explicit Testbed();
    ZTH_NO_COPY_NO_MOVE(Testbed)
    ~Testbed() override = default;

private:
    auto on_update() -> void override;
    auto on_event(const zth::Event& event) -> void override;
};
