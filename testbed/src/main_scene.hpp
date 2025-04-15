#pragma once

class MainScene : public zth::Scene
{
public:
    explicit MainScene();
    ZTH_NO_COPY_NO_MOVE(MainScene)
    ~MainScene() override = default;

private:
    zth::EntityHandle _camera = create_entity("Camera");
    zth::EntityHandle _cube = create_entity("Cube");

    std::shared_ptr<zth::Material> _cube_material = std::make_shared<zth::Material>();
    std::shared_ptr<zth::Material> _point_light_1_material =
        std::make_shared<zth::Material>(zth::Material{ .shader = zth::shaders::flat_color() });
    std::shared_ptr<zth::Material> _point_light_2_material =
        std::make_shared<zth::Material>(zth::Material{ .shader = zth::shaders::flat_color() });
    std::shared_ptr<zth::Material> _point_light_3_material =
        std::make_shared<zth::Material>(zth::Material{ .shader = zth::shaders::flat_color() });

    zth::EntityHandle _directional_light = create_entity("Directional Light");

    zth::EntityHandle _point_light_1 = create_entity("Point Light 1");
    zth::EntityHandle _point_light_2 = create_entity("Point Light 2");
    zth::EntityHandle _point_light_3 = create_entity("Point Light 3");

private:
    auto on_update() -> void override;
};
