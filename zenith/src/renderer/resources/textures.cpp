#include "zenith/renderer/resources/textures.hpp"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "zenith/core/assert.hpp"
#include "zenith/gl/texture.hpp"
#include "zenith/log/logger.hpp"

namespace zth::textures {

namespace {

std::shared_ptr<const gl::Texture2D> white_texture = nullptr;
std::shared_ptr<const gl::Texture2D> black_texture = nullptr;
std::shared_ptr<const gl::Texture2D> transparent_texture = nullptr;

constexpr std::array white_texture_data = { glm::vec3{ 1.0f } };
constexpr std::array black_texture_data = { glm::vec3{ 0.0f } };
constexpr std::array transparent_texture_data = { glm::vec4{ 0.0f } };

} // namespace

auto create() -> void
{
    ZTH_INTERNAL_TRACE("Creating textures...");

    // Textures should not be initialized when we call this function.
    ZTH_ASSERT(white_texture == nullptr);
    ZTH_ASSERT(black_texture == nullptr);
    ZTH_ASSERT(transparent_texture == nullptr);

    white_texture = std::make_shared<gl::Texture2D>(gl::Texture2D::from_rgb(white_texture_data, 1, 1));
    black_texture = std::make_shared<gl::Texture2D>(gl::Texture2D::from_rgb(black_texture_data, 1, 1));
    transparent_texture = std::make_shared<gl::Texture2D>(gl::Texture2D::from_rgba(transparent_texture_data, 1, 1));

    ZTH_INTERNAL_TRACE("Textures created...");
}

auto destroy() -> void
{
    ZTH_INTERNAL_TRACE("Destroying textures...");

    // Make sure all the textures were initialized.
    ZTH_ASSERT(white_texture != nullptr);
    ZTH_ASSERT(black_texture != nullptr);
    ZTH_ASSERT(transparent_texture != nullptr);

    white_texture.reset();
    black_texture.reset();
    transparent_texture.reset();

    ZTH_INTERNAL_TRACE("Textures destroyed...");
}

auto white() -> const std::shared_ptr<const gl::Texture2D>&
{
    ZTH_ASSERT(white_texture != nullptr);
    return white_texture;
}

auto black() -> const std::shared_ptr<const gl::Texture2D>&
{
    ZTH_ASSERT(black_texture != nullptr);
    return black_texture;
}

auto transparent() -> const std::shared_ptr<const gl::Texture2D>&
{
    ZTH_ASSERT(transparent_texture != nullptr);
    return transparent_texture;
}

} // namespace zth::textures
