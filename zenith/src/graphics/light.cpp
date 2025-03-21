#include "zenith/graphics/light.hpp"

#include "zenith/core/assert.hpp"
#include "zenith/log/formatters.hpp"

namespace zth {

auto to_string(LightType light_type) -> const char*
{
    switch (light_type)
    {
        using enum LightType;
    case Directional:
        return "Directional";
    case Point:
        return "Point";
    case Spot:
        return "Spot";
    case Ambient:
        return "Ambient";
    }

    ZTH_ASSERT(false);
    return "Unknown";
}

} // namespace zth

ZTH_DEFINE_FORMATTER(zth::LightType, light_type)
{
    return ZTH_FORMAT_OUT("{}", zth::to_string(light_type));
}

ZTH_DEFINE_FORMATTER(zth::LightProperties, properties)
{
    return ZTH_FORMAT_OUT("LightProperties {{\n"
                          "\t.color = {},\n"
                          "\t.ambient = {},\n"
                          "\t.diffuse = {},\n"
                          "\t.specular = {},\n"
                          "}}",
                          properties.color, properties.ambient, properties.diffuse, properties.specular);
}

ZTH_DEFINE_FORMATTER(zth::LightAttenuation, attenuation)
{
    return ZTH_FORMAT_OUT("LightAttenuation {{\n"
                          "\t.constant = {},\n"
                          "\t.linear = {},\n"
                          "\t.quadratic = {},\n"
                          "}}",
                          attenuation.constant, attenuation.linear, attenuation.quadratic);
}

ZTH_DEFINE_FORMATTER(zth::DirectionalLight, light)
{
    return ZTH_FORMAT_OUT("DirectionalLight {{\n"
                          "\t.properties = {},\n"
                          "}}",
                          light.properties);
}

ZTH_DEFINE_FORMATTER(zth::PointLight, light)
{
    return ZTH_FORMAT_OUT("PointLight {{\n"
                          "\t.properties = {},\n"
                          "\t.attenuation = {},\n"
                          "}}",
                          light.properties, light.attenuation);
}

ZTH_DEFINE_FORMATTER(zth::SpotLight, light)
{
    return ZTH_FORMAT_OUT("SpotLight {{\n"
                          "\t.inner_cutoff = {} deg,\n"
                          "\t.outer_cutoff = {} deg,\n"
                          "\t.properties = {},\n"
                          "\t.attenuation = {},\n"
                          "}}",
                          glm::degrees(glm::acos(light.inner_cutoff_cosine)),
                          glm::degrees(glm::acos(light.outer_cutoff_cosine)), light.properties, light.attenuation);
}

ZTH_DEFINE_FORMATTER(zth::AmbientLight, light)
{
    return ZTH_FORMAT_OUT("AmbientLight {{\n"
                          "\t.ambient = {},\n"
                          "}}",
                          light.ambient);
}
