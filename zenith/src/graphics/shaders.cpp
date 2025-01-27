#include "zenith/graphics/shaders.hpp"

#include "zenith/core/assert.hpp"
#include "zenith/embedded/shaders.hpp"
#include "zenith/log/logger.hpp"

namespace zth::shaders {

namespace {

std::unique_ptr<ShaderList> shader_list;

} // namespace

ShaderList::ShaderList()
    : fallback(embedded::shaders::fallback_vert, embedded::shaders::fallback_frag),
      flat_color(embedded::shaders::flat_color_vert, embedded::shaders::flat_color_frag),
      standard(embedded::shaders::standard_vert, embedded::shaders::standard_frag)
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
