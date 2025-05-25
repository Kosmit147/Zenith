#pragma once

namespace scripts {

class BallSpawner : public zth::Script
{
public:
    std::size_t count = 10;
    float spread = 10.0f;
    float min_height = 2.0f;
    float max_height = 20.0f;

public:
    BallSpawner() = default;
    ZTH_DEFAULT_COPY_DEFAULT_MOVE(BallSpawner)
    ~BallSpawner() override = default;

    [[nodiscard]] auto display_label() const -> const char* override;
    auto debug_edit() -> void override;

private:
    auto on_attach(zth::EntityHandle actor) -> void override;

    auto respawn() -> void;

private:
    zth::Vector<zth::EntityHandle> _balls;
};

} // namespace scripts
