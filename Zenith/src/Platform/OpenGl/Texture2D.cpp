#include "Zenith/Platform/OpenGl/Texture2D.hpp"

namespace zth {

Texture2D::Texture2D(Texture2D&& other) noexcept : _id(other._id)
{
    other._id = GL_NONE;
}

auto Texture2D::operator=(Texture2D&& other) noexcept -> Texture2D&
{
    _id = other._id;
    other._id = GL_NONE;
    return *this;
}

Texture2D::~Texture2D()
{
    destroy();
}

auto Texture2D::bind(u32 slot) const -> void
{
    glBindTextureUnit(slot, _id);
}

auto Texture2D::unbind(u32 slot) -> void
{
    glBindTextureUnit(slot, GL_NONE);
}

auto Texture2D::create() -> void
{
    glCreateTextures(GL_TEXTURE_2D, 1, &_id);
}

auto Texture2D::destroy() const -> void
{
    glDeleteTextures(1, &_id);
}

} // namespace zth
