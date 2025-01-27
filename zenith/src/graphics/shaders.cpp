#include "zenith/graphics/shaders.hpp"

#include <battery/embed.hpp>

#include "zenith/core/assert.hpp"
#include "zenith/log/logger.hpp"

namespace zth::shaders {

namespace {

// @cleanup: move to some kind of embedded module
const auto fallback_vertex_source = b::embed<"src/graphics/shaders/zth_fallback.vert">().str();
const auto fallback_fragment_source = b::embed<"src/graphics/shaders/zth_fallback.frag">().str();
const auto flat_color_vertex_source = b::embed<"src/graphics/shaders/zth_flat_color.vert">().str();
const auto flat_color_fragment_source = b::embed<"src/graphics/shaders/zth_flat_color.frag">().str();
const auto standard_vertex_source = b::embed<"src/graphics/shaders/zth_standard.vert">().str();
const auto standard_fragment_source = b::embed<"src/graphics/shaders/zth_standard.frag">().str();

std::unique_ptr<ShaderList> shader_list;

} // namespace

ShaderList::ShaderList()
    : fallback(fallback_vertex_source, fallback_fragment_source),
      flat_color(flat_color_vertex_source, flat_color_fragment_source),
      standard(standard_vertex_source, standard_fragment_source)
{}

auto load_shaders() -> void
{
    shader_list = std::make_unique<ShaderList>();
    ZTH_CORE_INFO("Shaders loaded.");
}

auto unload_shaders() -> void
{
    shader_list.reset();
    ZTH_CORE_INFO("Shaders unloaded.");
}

auto shaders() -> const ShaderList&
{
    ZTH_ASSERT(shader_list != nullptr);
    return *shader_list;
}

#define ZTH_SHADER_GETTER(shader_name)                                                                                 \
    auto shader_name() -> const Shader&                                                                                \
    {                                                                                                                  \
        ZTH_ASSERT(shader_list != nullptr);                                                                            \
        return shader_list->shader_name;                                                                               \
    }

ZTH_SHADER_GETTER(fallback);
ZTH_SHADER_GETTER(flat_color);
ZTH_SHADER_GETTER(standard);

} // namespace zth::shaders
