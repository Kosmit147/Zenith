#include "zenith/renderer/shaders.hpp"

#include "zenith/core/assert.hpp"
#include "zenith/embedded/shaders.hpp"
#include "zenith/log/logger.hpp"

namespace zth::shaders {

namespace {

std::unique_ptr<ShaderList> shader_list;

} // namespace

// clang-format off
ShaderList::ShaderList()
    : fallback({
          .vertex_source = embedded::shaders::fallback_vert,
          .fragment_source = embedded::shaders::fallback_frag }),
      flat_color({
          .vertex_source = embedded::shaders::flat_color_vert,
          .fragment_source = embedded::shaders::flat_color_frag }),
      standard({
          .vertex_source = embedded::shaders::standard_vert,
          .fragment_source = embedded::shaders::standard_frag })
{}
// clang-format on

auto load_shaders() -> void
{
    ZTH_INTERNAL_INFO("Loading shaders...");
    shader_list = std::make_unique<ShaderList>();
    ZTH_INTERNAL_INFO("Shaders loaded.");
}

auto unload_shaders() -> void
{
    ZTH_INTERNAL_INFO("Unloading shaders...");
    shader_list.reset();
    ZTH_INTERNAL_INFO("Shaders unloaded.");
}

auto shaders() -> const ShaderList&
{
    ZTH_ASSERT(shader_list != nullptr);
    return *shader_list;
}

#define ZTH_SHADER_GETTER(shader_name)                                                                                 \
    auto shader_name() -> const gl::Shader&                                                                            \
    {                                                                                                                  \
        ZTH_ASSERT(shader_list != nullptr);                                                                            \
        return shader_list->shader_name;                                                                               \
    }

ZTH_SHADER_GETTER(fallback);
ZTH_SHADER_GETTER(flat_color);
ZTH_SHADER_GETTER(standard);

} // namespace zth::shaders
