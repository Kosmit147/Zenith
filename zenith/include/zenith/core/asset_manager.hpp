#pragma once

#include <filesystem>
#include <ranges>

#include "zenith/gl/fwd.hpp"
#include "zenith/gl/texture.hpp"
#include "zenith/renderer/fwd.hpp"
#include "zenith/stl/string.hpp"
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

    static auto add_shader(StringView name, gl::Shader&& shader) -> Optional<Reference<gl::Shader>>;
    static auto add_shader_from_sources(StringView name, const gl::ShaderSources& sources)
        -> Optional<Reference<gl::Shader>>;
    static auto add_shader_from_files(StringView name, const gl::ShaderSourcePaths& paths)
        -> Optional<Reference<gl::Shader>>;

    static auto add_texture(StringView name, gl::Texture2D&& texture) -> Optional<Reference<gl::Texture2D>>;
    static auto add_texture_from_memory(StringView name, const void* data, usize data_size_bytes,
                                        const gl::TextureParams& params = {}) -> Optional<Reference<gl::Texture2D>>;
    static auto add_texture_from_memory(StringView name, std::ranges::contiguous_range auto&& data,
                                        const gl::TextureParams& params = {}) -> Optional<Reference<gl::Texture2D>>;
    static auto add_texture_from_file(const std::filesystem::path& path, const gl::TextureParams& params = {})
        -> Optional<Reference<gl::Texture2D>>;
    static auto add_texture_from_file(StringView name, const std::filesystem::path& path,
                                      const gl::TextureParams& params = {}) -> Optional<Reference<gl::Texture2D>>;

    static auto add_material(StringView name, const Material& material) -> Optional<Reference<Material>>;
    static auto add_material(StringView name, Material&& material) -> Optional<Reference<Material>>;

    [[nodiscard]] static auto get_shader(StringView name) -> Optional<Reference<gl::Shader>>;
    [[nodiscard]] static auto get_texture(StringView name) -> Optional<Reference<gl::Texture2D>>;
    [[nodiscard]] static auto get_material(StringView name) -> Optional<Reference<Material>>;

    [[nodiscard]] static auto get_shader_unchecked(StringView name) -> gl::Shader&;
    [[nodiscard]] static auto get_texture_unchecked(StringView name) -> gl::Texture2D&;
    [[nodiscard]] static auto get_material_unchecked(StringView name) -> Material&;

    static auto remove_shader(StringView name) -> bool;
    static auto remove_texture(StringView name) -> bool;
    static auto remove_material(StringView name) -> bool;

private:
    static StringHashMap<gl::Shader> _shaders;
    static StringHashMap<gl::Texture2D> _textures;
    static StringHashMap<Material> _materials;
};

auto AssetManager::add_texture_from_memory(StringView name, std::ranges::contiguous_range auto&& data,
                                           const gl::TextureParams& params) -> Optional<Reference<gl::Texture2D>>
{
    return add_texture_from_memory(name, std::data(data),
                                   std::size(data) * sizeof(std::ranges::range_value_t<decltype(data)>), params);
}

} // namespace zth
