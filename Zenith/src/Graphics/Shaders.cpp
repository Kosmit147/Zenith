#include "Zenith/Graphics/Shaders.hpp"

#include <battery/embed.hpp>

#include "Zenith/Logging/Logger.hpp"

namespace zth::shaders {

namespace {

const auto flat_color_vertex_source = b::embed<"src/Graphics/Shaders/flat_color.vert">().str();
const auto flat_color_fragment_source = b::embed<"src/Graphics/Shaders/flat_color.frag">().str();
const auto standard_vertex_source = b::embed<"src/Graphics/Shaders/standard.vert">().str();
const auto standard_fragment_source = b::embed<"src/Graphics/Shaders/standard.frag">().str();

std::optional<Shader> flat_color_local;
std::optional<Shader> standard_local;

} // namespace

auto load_shaders() -> void
{
    flat_color_local.emplace(flat_color_vertex_source, flat_color_fragment_source);
    flat_color = &*flat_color_local;

    standard_local.emplace(standard_vertex_source, standard_fragment_source);
    standard = &*standard_local;

    ZTH_CORE_INFO("Shaders loaded.");
}

auto unload_shaders() -> void
{
    flat_color_local.reset();
    flat_color = nullptr;

    standard_local.reset();
    standard = nullptr;

    ZTH_CORE_INFO("Shaders unloaded.");
}

} // namespace zth::shaders
