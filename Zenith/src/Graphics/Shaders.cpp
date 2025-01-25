#include "Zenith/Graphics/Shaders.hpp"

#include <battery/embed.hpp>

#include "Zenith/Core/Assert.hpp"
#include "Zenith/Logging/Logger.hpp"

namespace zth::shaders {

namespace {

// @cleanup: move to some kind of embedded module
const auto fallback_vertex_source = std::string_view(b::embed<"src/Graphics/Shaders/zth_fallback.vert">().data());
const auto fallback_fragment_source = std::string_view(b::embed<"src/Graphics/Shaders/zth_fallback.frag">().data());
const auto flat_color_vertex_source = std::string_view(b::embed<"src/Graphics/Shaders/zth_flat_color.vert">().data());
const auto flat_color_fragment_source = std::string_view(b::embed<"src/Graphics/Shaders/zth_flat_color.frag">().data());
const auto standard_vertex_source = std::string_view(b::embed<"src/Graphics/Shaders/zth_standard.vert">().data());
const auto standard_fragment_source = std::string_view(b::embed<"src/Graphics/Shaders/zth_standard.frag">().data());

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
