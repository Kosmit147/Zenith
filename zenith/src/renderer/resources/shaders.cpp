#include "zenith/renderer/resources/shaders.hpp"

#include "zenith/core/assert.hpp"
#include "zenith/embedded/shaders.hpp"
#include "zenith/gl/shader.hpp"
#include "zenith/log/logger.hpp"

namespace zth::shaders {

namespace {

ShadersArray shaders_array;

} // namespace

auto load() -> void
{
    ZTH_INTERNAL_TRACE("Loading shaders...");

#if defined(ZTH_ASSERTIONS)
    for (auto& shader : shaders_array)
    {
        // Shaders should not be initialized when we call this function.
        ZTH_ASSERT(shader == nullptr);
    }
#endif

    shaders_array[fallback_shader_index] =
        std::make_shared<gl::Shader>(gl::ShaderSources{ .vertex_source = embedded::shaders::fallback_vert,
                                                        .fragment_source = embedded::shaders::fallback_frag });

    shaders_array[flat_color_shader_index] =
        std::make_shared<gl::Shader>(gl::ShaderSources{ .vertex_source = embedded::shaders::flat_color_vert,
                                                        .fragment_source = embedded::shaders::flat_color_frag });

    shaders_array[standard_shader_index] =
        std::make_shared<gl::Shader>(gl::ShaderSources{ .vertex_source = embedded::shaders::standard_vert,
                                                        .fragment_source = embedded::shaders::standard_frag });

#if defined(ZTH_ASSERTIONS)
    for (auto& shader : shaders_array)
    {
        // Make sure all the shaders were initialized.
        ZTH_ASSERT(shader != nullptr);
    }
#endif

    ZTH_INTERNAL_TRACE("Shaders loaded.");
}

auto unload() -> void
{
    ZTH_INTERNAL_TRACE("Unloading shaders...");

    for (auto& shader : shaders_array)
    {
        ZTH_ASSERT(shader != nullptr); // All the shaders should have been initialized.
        shader.reset();
    }

    ZTH_INTERNAL_TRACE("Shaders unloaded.");
}

auto all() -> const ShadersArray&
{
    return shaders_array;
}

#define ZTH_SHADER_GETTER(shader_name)                                                                                 \
    auto shader_name() -> const std::shared_ptr<const gl::Shader>&                                                     \
    {                                                                                                                  \
        ZTH_ASSERT(shaders_array[shader_name##_shader_index] != nullptr);                                              \
        return shaders_array[shader_name##_shader_index];                                                              \
    }

ZTH_SHADER_GETTER(fallback);
ZTH_SHADER_GETTER(flat_color);
ZTH_SHADER_GETTER(standard);

} // namespace zth::shaders
