#pragma once

#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

#include "zenith/math/vector.hpp"

namespace zth {

struct LightProperties
{
    glm::vec3 color{ 1.0f };
    glm::vec3 ambient{ 0.2f };
    glm::vec3 diffuse{ 0.5f };
    glm::vec3 specular{ 1.0f };
};

// example light attenuation values:
// Distance     Constant    Linear      Quadratic
// 7	        1.0	    0.7	        1.8
// 13	        1.0	    0.35	0.44
// 20	        1.0	    0.22	0.20
// 32	        1.0	    0.14	0.07
// 50	        1.0	    0.09	0.032
// 65	        1.0	    0.07	0.017
// 100	        1.0	    0.045	0.0075
// 160	        1.0	    0.027	0.0028
// 200	        1.0	    0.022	0.0019
// 325	        1.0	    0.014	0.0007
// 600	        1.0	    0.007	0.0002
// 3250	        1.0	    0.0014	0.000007

struct LightAttenuation
{
    float constant = 1.0f;
    float linear = 0.22f;
    float quadratic = 0.20f;
};

struct DirectionalLight
{
    glm::vec3 direction{ math::world_down };
    LightProperties properties{};
};

struct PointLight
{
    glm::vec3 position{ 0.0f };
    LightProperties properties{};
    LightAttenuation attenuation{};
};

struct SpotLight
{
    glm::vec3 position{ 0.0f };
    glm::vec3 direction{ math::world_down };
    float inner_cutoff = glm::cos(glm::radians(12.5f)); // cos of the inner cone angle
    float outer_cutoff = glm::cos(glm::radians(15.0f)); // cos of the outer cone angle
    LightProperties properties{};
    LightAttenuation attenuation{};
};

struct AmbientLight
{
    glm::vec3 ambient{ 0.1f };
};

} // namespace zth
