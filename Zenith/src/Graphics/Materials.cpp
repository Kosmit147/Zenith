#include "Zenith/Graphics/Materials.hpp"

#include <glm/vec3.hpp>

#include "Zenith/Core/Typedefs.hpp"
#include "Zenith/Graphics/Shaders.hpp"

namespace zth::materials {

Material plain = {
    // default values
};

Material emerald = {
    .ambient = glm::vec3{ 0.0215f, 0.1745f, 0.0215f },
    .diffuse = glm::vec3{ 0.07568f, 0.61424f, 0.07568f },
    .specular = glm::vec3{ 0.633f, 0.727811f, 0.633f },
    .shininess = 76.8f,
};

Material jade = {
    .ambient = glm::vec3{ 0.135f, 0.2225f, 0.1575f },
    .diffuse = glm::vec3{ 0.54f, 0.89f, 0.63f },
    .specular = glm::vec3{ 0.316228f, 0.316228f, 0.316228f },
    .shininess = 12.8f,
};

Material obsidian = {
    .ambient = glm::vec3{ 0.05375f, 0.05f, 0.06625f },
    .diffuse = glm::vec3{ 0.18275f, 0.17f, 0.22525f },
    .specular = glm::vec3{ 0.332741f, 0.328634f, 0.346435f },
    .shininess = 38.4f,
};

Material pearl = {
    .ambient = glm::vec3{ 0.25f, 0.20725f, 0.20725f },
    .diffuse = glm::vec3{ 1.0f, 0.829f, 0.829f },
    .specular = glm::vec3{ 0.296648f, 0.296648f, 0.296648f },
    .shininess = 11.264f,
};

Material ruby = {
    .ambient = glm::vec3{ 0.1745f, 0.01175f, 0.01175f },
    .diffuse = glm::vec3{ 0.61424f, 0.04136f, 0.04136f },
    .specular = glm::vec3{ 0.727811f, 0.626959f, 0.626959f },
    .shininess = 76.8f,
};

Material turquoise = {
    .ambient = glm::vec3{ 0.1f, 0.18725f, 0.1745f },
    .diffuse = glm::vec3{ 0.396f, 0.74151f, 0.69102f },
    .specular = glm::vec3{ 0.297254f, 0.30829f, 0.306678f },
    .shininess = 12.8f,
};

Material brass = {
    .ambient = glm::vec3{ 0.329412f, 0.223529f, 0.027451f },
    .diffuse = glm::vec3{ 0.780392f, 0.568627f, 0.113725f },
    .specular = glm::vec3{ 0.992157f, 0.941176f, 0.807843f },
    .shininess = 27.89743616f,
};

Material bronze = {
    .ambient = glm::vec3{ 0.2125f, 0.1275f, 0.054f },
    .diffuse = glm::vec3{ 0.714f, 0.4284f, 0.18144f },
    .specular = glm::vec3{ 0.393548f, 0.271906f, 0.166721f },
    .shininess = 25.6f,
};

Material chrome = {
    .ambient = glm::vec3{ 0.25f, 0.25f, 0.25f },
    .diffuse = glm::vec3{ 0.4f, 0.4f, 0.4f },
    .specular = glm::vec3{ 0.774597f, 0.774597f, 0.774597f },
    .shininess = 76.8f,
};

Material copper = {
    .ambient = glm::vec3{ 0.19125f, 0.0735f, 0.0225f },
    .diffuse = glm::vec3{ 0.7038f, 0.27048f, 0.0828f },
    .specular = glm::vec3{ 0.256777f, 0.137622f, 0.086014f },
    .shininess = 12.8f,
};

Material gold = {
    .ambient = glm::vec3{ 0.24725f, 0.1995f, 0.0745f },
    .diffuse = glm::vec3{ 0.75164f, 0.60648f, 0.22648f },
    .specular = glm::vec3{ 0.628281f, 0.555802f, 0.366065f },
    .shininess = 51.2f,
};

Material silver = {
    .ambient = glm::vec3{ 0.19225f, 0.19225f, 0.19225f },
    .diffuse = glm::vec3{ 0.50754f, 0.50754f, 0.50754f },
    .specular = glm::vec3{ 0.508273f, 0.508273f, 0.508273f },
    .shininess = 51.2f,
};

Material black_plastic = {
    .ambient = glm::vec3{ 0.0f, 0.0f, 0.0f },
    .diffuse = glm::vec3{ 0.01f, 0.01f, 0.01f },
    .specular = glm::vec3{ 0.50f, 0.50f, 0.50f },
    .shininess = 32.0f,
};

Material cyan_plastic = {
    .ambient = glm::vec3{ 0.0f, 0.1f, 0.06f },
    .diffuse = glm::vec3{ 0.0f, 0.50980392f, 0.50980392f },
    .specular = glm::vec3{ 0.50196078f, 0.50196078f, 0.50196078f },
    .shininess = 32.0f,
};

Material green_plastic = {
    .ambient = glm::vec3{ 0.0f, 0.0f, 0.0f },
    .diffuse = glm::vec3{ 0.1f, 0.35f, 0.1f },
    .specular = glm::vec3{ 0.45f, 0.55f, 0.45f },
    .shininess = 32.0f,
};

Material red_plastic = {
    .ambient = glm::vec3{ 0.0f, 0.0f, 0.0f },
    .diffuse = glm::vec3{ 0.5f, 0.0f, 0.0f },
    .specular = glm::vec3{ 0.7f, 0.6f, 0.6f },
    .shininess = 32.0f,
};

Material white_plastic = {
    .ambient = glm::vec3{ 0.0f, 0.0f, 0.0f },
    .diffuse = glm::vec3{ 0.55f, 0.55f, 0.55f },
    .specular = glm::vec3{ 0.70f, 0.70f, 0.70f },
    .shininess = 32.0f,
};

Material yellow_plastic = {
    .ambient = glm::vec3{ 0.0f, 0.0f, 0.0f },
    .diffuse = glm::vec3{ 0.5f, 0.5f, 0.0f },
    .specular = glm::vec3{ 0.60f, 0.60f, 0.50f },
    .shininess = 32.0f,
};

Material black_rubber = {
    .ambient = glm::vec3{ 0.02f, 0.02f, 0.02f },
    .diffuse = glm::vec3{ 0.01f, 0.01f, 0.01f },
    .specular = glm::vec3{ 0.4f, 0.4f, 0.4f },
    .shininess = 10.0f,
};

Material cyan_rubber = {
    .ambient = glm::vec3{ 0.0f, 0.05f, 0.05f },
    .diffuse = glm::vec3{ 0.4f, 0.5f, 0.5f },
    .specular = glm::vec3{ 0.04f, 0.7f, 0.7f },
    .shininess = 10.0f,
};

Material green_rubber = {
    .ambient = glm::vec3{ 0.0f, 0.05f, 0.0f },
    .diffuse = glm::vec3{ 0.4f, 0.5f, 0.4f },
    .specular = glm::vec3{ 0.04f, 0.7f, 0.04f },
    .shininess = 10.0f,
};

Material red_rubber = {
    .ambient = glm::vec3{ 0.05f, 0.0f, 0.0f },
    .diffuse = glm::vec3{ 0.5f, 0.4f, 0.4f },
    .specular = glm::vec3{ 0.7f, 0.04f, 0.04f },
    .shininess = 10.0f,
};

Material white_rubber = {
    .ambient = glm::vec3{ 0.05f, 0.05f, 0.05f },
    .diffuse = glm::vec3{ 0.5f, 0.5f, 0.5f },
    .specular = glm::vec3{ 0.7f, 0.7f, 0.7f },
    .shininess = 10.0f,
};

Material yellow_rubber = {
    .ambient = glm::vec3{ 0.05f, 0.05f, 0.0f },
    .diffuse = glm::vec3{ 0.5f, 0.5f, 0.4f },
    .specular = glm::vec3{ 0.7f, 0.7f, 0.04f },
    .shininess = 10.0f,
};

auto init_materials() -> void
{
    for (auto* material : materials)
        material->shader = shaders::standard;

    for (usize i = 1; i < materials.size(); i++)
    {
        // temp fix for wrong material values
        // current values assume light ambient, diffuse and specular values of 1.0
        // TODO: fix the values

        materials[i]->ambient *= 5.0f;
        materials[i]->diffuse *= 2.0f;
    }
}

} // namespace zth::materials
