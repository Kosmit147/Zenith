#pragma once

namespace zth {

class ImGuiRenderer
{
public:
    ImGuiRenderer() = delete;

    static auto init() -> void;
    static auto on_update() -> void;
    static auto on_render() -> void;
    static auto shut_down() -> void;
};

} // namespace zth