#pragma once

class CubeScript : public zth::Script
{
public:
    CubeScript() = default;
    ~CubeScript() override = default;

private:
    auto on_update(zth::EntityHandle actor) -> void override;
};