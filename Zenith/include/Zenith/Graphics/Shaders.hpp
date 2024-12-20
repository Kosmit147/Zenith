#pragma once

#include "Zenith/Platform/OpenGl/Shader.hpp"
#include "Zenith/Utility/Macros.hpp"

namespace zth::shaders {

struct ShaderList
{
    Shader flat_color;
    Shader standard;

    explicit ShaderList();
    ZTH_NO_COPY_NO_MOVE(ShaderList)
    ~ShaderList() = default;
};

auto load_shaders() -> void;
auto unload_shaders() -> void;

[[nodiscard]] auto shaders() -> const ShaderList&;

[[nodiscard]] auto flat_color() -> const Shader&;
[[nodiscard]] auto standard() -> const Shader&;

} // namespace zth::shaders
