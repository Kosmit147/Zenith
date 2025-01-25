#pragma once

#include "zenith/platform/gl/shader.hpp"
#include "zenith/utility/macros.hpp"

namespace zth::shaders {

struct ShaderList
{
    Shader fallback;
    Shader flat_color;
    Shader standard;

    explicit ShaderList();
    ZTH_NO_COPY_NO_MOVE(ShaderList)
    ~ShaderList() = default;
};

auto load_shaders() -> void;
auto unload_shaders() -> void;

[[nodiscard]] auto shaders() -> const ShaderList&;

[[nodiscard]] auto fallback() -> const Shader&;
[[nodiscard]] auto flat_color() -> const Shader&;
[[nodiscard]] auto standard() -> const Shader&;

} // namespace zth::shaders
