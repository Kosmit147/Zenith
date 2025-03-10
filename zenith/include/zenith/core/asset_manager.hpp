#pragma once

#include <filesystem>
#include <functional>
#include <optional>
#include <ranges>
#include <string_view>

#include "zenith/gl/fwd.hpp"
#include "zenith/gl/texture.hpp"
#include "zenith/stl/string_map.hpp"

namespace zth {

class AssetManager
{
    // @test: loading assets from files and Asset Manager in general

public:
    using ShaderRef = std::reference_wrapper<gl::Shader>;
    using TextureRef = std::reference_wrapper<gl::Texture2D>;

    AssetManager() = delete;

    static auto init() -> void;
    static auto shut_down() -> void;

    static auto add_shader(std::string_view name, gl::Shader&& shader) -> std::optional<ShaderRef>;
    static auto add_shader_from_sources(std::string_view name, const gl::ShaderSources& sources)
        -> std::optional<ShaderRef>;
    static auto add_shader_from_files(std::string_view name, const gl::ShaderSourcePaths& paths)
        -> std::optional<ShaderRef>;

    static auto add_texture(std::string_view name, gl::Texture2D&& texture) -> std::optional<TextureRef>;
    static auto add_texture_from_memory(std::string_view name, const void* data, usize data_size_bytes,
                                        const gl::TextureParams& params = {}) -> std::optional<TextureRef>;
    static auto add_texture_from_memory(std::string_view name, std::ranges::contiguous_range auto&& data,
                                        const gl::TextureParams& params = {}) -> std::optional<TextureRef>;
    static auto add_texture_from_file(const std::filesystem::path& path, const gl::TextureParams& params = {})
        -> std::optional<TextureRef>;
    static auto add_texture_from_file(std::string_view name, const std::filesystem::path& path,
                                      const gl::TextureParams& params = {}) -> std::optional<TextureRef>;

    [[nodiscard]] static auto get_shader(std::string_view name) -> std::optional<ShaderRef>;
    [[nodiscard]] static auto get_texture(std::string_view name) -> std::optional<TextureRef>;

    static auto remove_shader(std::string_view name) -> bool;
    static auto remove_texture(std::string_view name) -> bool;

private:
    static StringHashMap<gl::Shader> _shaders;
    static StringHashMap<gl::Texture2D> _textures;
};

auto AssetManager::add_texture_from_memory(std::string_view name, std::ranges::contiguous_range auto&& data,
                                           const gl::TextureParams& params) -> std::optional<TextureRef>
{
    return add_texture_from_memory(name, std::data(data),
                                   std::size(data) * sizeof(std::ranges::range_value_t<decltype(data)>), params);
}

} // namespace zth
