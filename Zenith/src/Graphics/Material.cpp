#include "Zenith/Graphics/Material.hpp"

#include "Zenith/Core/Assert.hpp"
#include "Zenith/Graphics/Shaders.hpp"
#include "Zenith/Platform/OpenGl/Texture2D.hpp"

namespace zth {

Material::Material(const Shader* shader, const Texture2D* texture)
{
    set_shader(shader);
    set_texture(texture);
}

auto Material::bind() const -> void
{
    ZTH_ASSERT(_shader != nullptr);
    _shader->bind();

    if (_texture)
    {
        _texture->bind();
        _shader->set_unif("tex", 0);
    }
}

auto Material::set_shader(const Shader* shader) -> void
{
    _shader = shader ? shader : shaders::fallback_shader;
}

auto Material::set_texture(const Texture2D* texture) -> void
{
    _texture = texture;
}

} // namespace zth
