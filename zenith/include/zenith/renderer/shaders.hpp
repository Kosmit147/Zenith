#pragma once

#include "zenith/gl/shader.hpp"
#include "zenith/util/macros.hpp"

namespace zth::shaders {

struct ShaderList
{
    gl::Shader fallback;
    gl::Shader flat_color;
    gl::Shader standard;

    explicit ShaderList();
    ZTH_NO_COPY_NO_MOVE(ShaderList)
    ~ShaderList() = default;
};

auto load_shaders() -> void;
auto unload_shaders() -> void;

[[nodiscard]] auto shaders() -> const ShaderList&;

[[nodiscard]] auto fallback() -> const gl::Shader&;
[[nodiscard]] auto flat_color() -> const gl::Shader&;
[[nodiscard]] auto standard() -> const gl::Shader&;

} // namespace zth::shaders
