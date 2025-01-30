#pragma once

#include <functional>
#include <optional>
#include <string_view>

#include "zenith/gl/shader.hpp"
#include "zenith/stl/string_map.hpp"

namespace zth {

class AssetManager
{
    // @test: loading assets from files

public:
    using ShaderRef = std::reference_wrapper<gl::Shader>;

    AssetManager() = delete;

    static auto init() -> void;
    static auto shut_down() -> void;

    static auto add_shader(std::string_view name, gl::Shader&& shader) -> std::optional<ShaderRef>;
    static auto add_shader_from_sources(std::string_view name,
                                        const gl::ShaderSources& sources) -> std::optional<ShaderRef>;
    static auto add_shader_from_files(std::string_view name,
                                      const gl::ShaderSourcePaths& paths) -> std::optional<ShaderRef>;

    [[nodiscard]] static auto get_shader(std::string_view name) -> std::optional<ShaderRef>;

    static auto remove_shader(std::string_view name) -> bool;

private:
    static StringHashMap<gl::Shader> _shaders;
};

} // namespace zth
