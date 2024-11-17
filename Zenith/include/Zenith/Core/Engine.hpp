#pragma once

#include "Zenith/Platform/Window.hpp"

namespace zth {

class Engine
{
public:
    Window window;

public:
    static auto init(const WindowSpec& window_spec = {}) -> void;
    [[nodiscard]] static auto get() -> Engine&;
    static auto shut_down() -> void;

private:
    explicit Engine(const WindowSpec& window_spec);
};

} // namespace zth
