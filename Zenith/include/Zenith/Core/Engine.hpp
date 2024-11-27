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

    auto set_time(double val) { _time = val; }
    auto set_delta_time(double val) { _delta_time = val; }
    [[nodiscard]] auto time() const { return _time; }
    [[nodiscard]] auto delta_time() const { return _delta_time; }

private:
    double _delta_time = 0.0;
    double _time = 0.0;

private:
    explicit Engine(const WindowSpec& window_spec);
};

} // namespace zth
