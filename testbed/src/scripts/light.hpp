#pragma once

namespace scripts {

// Changes the albedo of the provided material to match the color of the light.

class Light : public zth::Script
{
public:
    explicit Light(std::shared_ptr<zth::Material> light_material);

    auto on_update(zth::EntityHandle actor) -> void override;

private:
    std::shared_ptr<zth::Material> _light_material;
};

} // namespace scripts
