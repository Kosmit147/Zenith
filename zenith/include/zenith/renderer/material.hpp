#pragma once

#include <glm/vec3.hpp>

#include <memory>

#include "zenith/gl/fwd.hpp"
#include "zenith/renderer/shaders.hpp"

namespace zth {

struct Material
{
    std::shared_ptr<const gl::Shader> shader = shaders::standard();

    glm::vec3 albedo{ 1.0f };
    std::shared_ptr<const gl::Texture2D> diffuse_map = nullptr;
    std::shared_ptr<const gl::Texture2D> specular_map = nullptr;
    std::shared_ptr<const gl::Texture2D> emission_map = nullptr;

    glm::vec3 ambient{ 0.2f };
    glm::vec3 diffuse{ 1.0f };
    glm::vec3 specular{ 0.6f };
    float shininess = 32.0f;
};

} // namespace zth
