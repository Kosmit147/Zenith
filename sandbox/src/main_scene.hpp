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
    zth::EntityHandle _light = create_entity("Light");

    std::shared_ptr<zth::gl::Texture2D> _cube_texture;
    std::shared_ptr<zth::Material> _cube_material;

private:
    auto on_load() -> void override;
};
