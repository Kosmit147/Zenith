#pragma once

namespace scripts {

class Cube : public zth::Script
{
public:
    Cube() = default;
    ~Cube() override = default;

    auto on_update(zth::EntityHandle actor) -> void override;
};

} // namespace scripts
