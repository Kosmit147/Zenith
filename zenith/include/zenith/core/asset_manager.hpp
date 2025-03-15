#pragma once

#include <filesystem>
#include <ranges>
#include <string_view>

#include "zenith/gl/fwd.hpp"
#include "zenith/gl/texture.hpp"
#include "zenith/stl/string_map.hpp"
#include "zenith/util/optional.hpp"
#include "zenith/util/reference.hpp"

namespace zth {

class AssetManager
{
    // @test: Loading assets from files and AssetManager in general.

public:
    AssetManager() = delete;

    static auto init() -> void;
    static auto shut_down() -> void;

    static auto add_shader(std::string_view name, gl::Shader&& shader) -> Optional<Reference<gl::Shader>>;
    static auto add_shader_from_sources(std::string_view name, const gl::ShaderSources& sources)
        -> Optional<Reference<gl::Shader>>;
    static auto add_shader_from_files(std::string_view name, const gl::ShaderSourcePaths& paths)
        -> Optional<Reference<gl::Shader>>;

    static auto add_texture(std::string_view name, gl::Texture2D&& texture) -> Optional<Reference<gl::Texture2D>>;
    static auto add_texture_from_memory(std::string_view name, const void* data, usize data_size_bytes,
                                        const gl::TextureParams& params = {}) -> Optional<Reference<gl::Texture2D>>;
    static auto add_texture_from_memory(std::string_view name, std::ranges::contiguous_range auto&& data,
                                        const gl::TextureParams& params = {}) -> Optional<Reference<gl::Texture2D>>;
    static auto add_texture_from_file(const std::filesystem::path& path, const gl::TextureParams& params = {})
        -> Optional<Reference<gl::Texture2D>>;
    static auto add_texture_from_file(std::string_view name, const std::filesystem::path& path,
                                      const gl::TextureParams& params = {}) -> Optional<Reference<gl::Texture2D>>;

    [[nodiscard]] static auto get_shader(std::string_view name) -> Optional<Reference<gl::Shader>>;
    [[nodiscard]] static auto get_texture(std::string_view name) -> Optional<Reference<gl::Texture2D>>;

    static auto remove_shader(std::string_view name) -> bool;
    static auto remove_texture(std::string_view name) -> bool;

private:
    static StringHashMap<gl::Shader> _shaders;
    static StringHashMap<gl::Texture2D> _textures;
};

auto AssetManager::add_texture_from_memory(std::string_view name, std::ranges::contiguous_range auto&& data,
                                           const gl::TextureParams& params) -> Optional<Reference<gl::Texture2D>>
{
    return add_texture_from_memory(name, std::data(data),
                                   std::size(data) * sizeof(std::ranges::range_value_t<decltype(data)>), params);
}

} // namespace zth
