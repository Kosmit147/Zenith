#pragma once

#include "zenith/stl/string.hpp"
#include "zenith/util/result.hpp"

namespace zth {

class ImGuiRenderer
{
public:
    static constexpr auto initial_font_scale = 1.5f;

public:
    ImGuiRenderer() = delete;

    [[nodiscard]] static auto init() -> Result<void, String>;
    static auto start_frame() -> void;
    static auto render() -> void;
    static auto shut_down() -> void;
};

} // namespace zth
