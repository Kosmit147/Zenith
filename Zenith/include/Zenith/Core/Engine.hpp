#pragma once

#include "Zenith/Platform/Window.hpp"
#include "Zenith/Graphics/Renderer.hpp"

namespace zth {

class Engine
{
public:
    Window window;
    Renderer renderer;

public:
    static auto init(const WindowSpec& window_spec = {}) -> void;
    [[nodiscard]] static auto get() -> Engine&;
    static auto shut_down() -> void;

    auto on_update() -> void;

private:
    explicit Engine(const WindowSpec& window_spec);
};

} // namespace zth
