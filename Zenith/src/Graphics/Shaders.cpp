#include "Zenith/Graphics/Shaders.hpp"

#include <battery/embed.hpp>

#include "Zenith/Logging/Logger.hpp"

namespace zth::shaders {

namespace {

const auto fallback_shader_vertex_source = b::embed<"src/Graphics/Shaders/fallback.vert">().str();
const auto fallback_shader_fragment_source = b::embed<"src/Graphics/Shaders/fallback.frag">().str();
const auto texture_shader_vertex_source = b::embed<"src/Graphics/Shaders/texture.vert">().str();
const auto texture_shader_fragment_source = b::embed<"src/Graphics/Shaders/texture.frag">().str();

std::optional<Shader> fallback_shader_local;
std::optional<Shader> texture_shader_local;

} // namespace

auto load_shaders() -> void
{
    texture_shader_local.emplace(texture_shader_vertex_source, texture_shader_fragment_source);
    texture_shader = &texture_shader_local.value();

    fallback_shader_local.emplace(fallback_shader_vertex_source, fallback_shader_fragment_source);
    fallback_shader = &fallback_shader_local.value();

    ZTH_CORE_INFO("Shaders loaded.");
}

auto unload_shaders() -> void
{
    texture_shader_local.reset();
    texture_shader = nullptr;

    fallback_shader_local.reset();
    fallback_shader = nullptr;

    ZTH_CORE_INFO("Shaders unloaded.");
}

} // namespace zth::shaders
