#pragma once

#include <filesystem>
#include <functional>
#include <optional>
#include <string_view>

#include "zenith/platform/gl/shader.hpp"
#include "zenith/utility/string_map.hpp"

namespace zth {

class AssetManager
{
public:
    using ShaderRef = std::reference_wrapper<Shader>;

    AssetManager() = delete;

    static auto init() -> void;
    static auto shut_down() -> void;

    static auto load_shader_from_file(std::string_view name, const std::filesystem::path& vertex_path,
                                      const std::filesystem::path& fragment_path) -> std::optional<ShaderRef>;
    static auto load_shader_from_memory(std::string_view name, std::string_view vertex_source,
                                        std::string_view fragment_source) -> std::optional<ShaderRef>;

    [[nodiscard]] static auto get_shader(std::string_view name) -> std::optional<ShaderRef>;

    static auto unload_shader(std::string_view name) -> bool;

private:
    static StringHashMap<Shader> _shaders;
};

} // namespace zth
