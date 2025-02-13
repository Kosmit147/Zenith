#version 460 core

#include "zth_defines.glsl"

struct LightProperties
{
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct LightAttenuation
{
    float constant;
    float linear;
    float quadratic;
};

struct Light
{
    vec3 direction;
    LightProperties properties;
    float strength;
};

struct DirectionalLight
{
    vec3 direction;
    LightProperties properties;
};

struct PointLight
{
    vec3 position;
    LightProperties properties;
    LightAttenuation attenuation;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    float inner_cutoff;
    float outer_cutoff;
    LightProperties properties;
    LightAttenuation attenuation;
};

struct AmbientLight
{
    vec3 ambient;
};

in vec3 Position;
in vec3 Normal;
in vec2 UV;

layout (std140, binding = ZTH_CAMERA_UBO_BINDING_POINT) uniform CameraUbo
{
    mat4 view_projection;
    vec3 position;
} camera;

layout (std430, binding = ZTH_DIRECTIONAL_LIGHTS_SSBO_BINDING_POINT) restrict readonly buffer DirectionalLightsSsbo
{
    uint count;
    DirectionalLight lights[];
} directional_lights;

layout (std430, binding = ZTH_POINT_LIGHTS_SSBO_BINDING_POINT) restrict readonly buffer PointLightsSsbo
{
    uint count;
    PointLight lights[];
} point_lights;

layout (std430, binding = ZTH_SPOT_LIGHTS_SSBO_BINDING_POINT) restrict readonly buffer SpotLightsSsbo
{
    uint count;
    SpotLight lights[];
} spot_lights;

layout (std430, binding = ZTH_AMBIENT_LIGHTS_SSBO_BINDING_POINT) restrict readonly buffer AmbientLightsSsbo
{
    uint count;
    AmbientLight lights[];
} ambient_lights;

layout (std140, binding = ZTH_MATERIAL_UBO_BINDING_POINT) uniform MaterialUbo
{
    vec3 albedo;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;

    bool has_diffuse_map;
    bool has_specular_map;
    bool has_emission_map;
} material;

layout (binding = ZTH_DIFFUSE_MAP_SLOT) uniform sampler2D diffuse_map;
layout (binding = ZTH_SPECULAR_MAP_SLOT) uniform sampler2D specular_map;
layout (binding = ZTH_EMISSION_MAP_SLOT) uniform sampler2D emission_map;

out vec4 out_color;

float calc_strength(LightAttenuation attenuation, float dist)
{
    return 1.0 / (attenuation.constant + attenuation.linear * dist + attenuation.quadratic * (dist * dist));
}

Light convert_directional_light(DirectionalLight directional_light)
{
    return Light(normalize(directional_light.direction), directional_light.properties, 1.0);
}

Light convert_point_light(PointLight point_light)
{
    vec3 diff = Position - point_light.position;
    float dist = length(diff);
    float strength = calc_strength(point_light.attenuation, dist);
    vec3 point_light_direction = normalize(diff);

    return Light(point_light_direction, point_light.properties, strength);
}

// dist is the distance from frag position to spot light
// angle is the dot product of the direction from spot light to frag position and spot light direction
Light convert_spot_light(SpotLight spot_light, float dist, float angle)
{
    float strength = calc_strength(spot_light.attenuation, dist);

    float intensity = (angle - spot_light.outer_cutoff) / (spot_light.inner_cutoff - spot_light.outer_cutoff);
    intensity = clamp(intensity, 0.0, 1.0);
    strength *= intensity;

    return Light(normalize(spot_light.direction), spot_light.properties, strength);
}

vec3 calc_light_ambient(Light light)
{
    return light.properties.ambient;
}

vec3 calc_light_diffuse(Light light, vec3 normal)
{
    return light.properties.diffuse * max(dot(-light.direction, normal), 0.0);
}

vec3 calc_light_specular(Light light, vec3 normal, vec3 view_direction)
{
    vec3 reflection = reflect(light.direction, normal);
    vec3 specular_factor = light.properties.specular * pow(max(dot(reflection, -view_direction), 0.0), material.shininess);

    if (material.has_specular_map)
        specular_factor *= vec3(texture(specular_map, UV));

    return specular_factor;
}

vec3 calc_light(Light light, vec3 normal, vec3 view_direction)
{
    vec3 ambient = calc_light_ambient(light) * material.ambient;
    vec3 diffuse = calc_light_diffuse(light, normal) * material.diffuse;
    vec3 specular = calc_light_specular(light, normal, view_direction) * material.specular;
    return (ambient + diffuse + specular) * light.properties.color * light.strength;
}

vec3 calc_directional_lights(vec3 normal, vec3 view_direction)
{
    vec3 result = vec3(0.0);

    for (uint i = 0; i < directional_lights.count; i++)
    {
        Light directional_light = convert_directional_light(directional_lights.lights[i]);
        result += calc_light(directional_light, normal, view_direction);
    }

    return result;
}

vec3 calc_point_lights(vec3 normal, vec3 view_direction)
{
    vec3 result = vec3(0.0);

    for (uint i = 0; i < point_lights.count; i++)
    {
        Light light = convert_point_light(point_lights.lights[i]);
        result += calc_light(light, normal, view_direction);
    }

    return result;
}

vec3 calc_spot_lights(vec3 normal, vec3 view_direction)
{
    vec3 result = vec3(0.0);

    for (uint i = 0; i < spot_lights.count; i++)
    {
        SpotLight spot_light = spot_lights.lights[i];
	    vec3 diff_from_spot_light = Position - spot_light.position;
	    float distance_from_spot_light = length(diff_from_spot_light);
        vec3 normalized_diff = diff_from_spot_light / distance_from_spot_light;
	    float angle = dot(spot_light.direction, normalized_diff);

        if (angle > spot_light.outer_cutoff)
        {
            Light light = convert_spot_light(spot_light, distance_from_spot_light, angle);
            result += calc_light(light, normal, view_direction);
        }
    }

    return result;
}

vec3 calc_ambient_lights()
{
    vec3 result = vec3(0.0);

    for (uint i = 0; i < ambient_lights.count; i++)
        result += ambient_lights.lights[i].ambient;

    return result;
}

void main()
{
    vec4 object_color = vec4(material.albedo, 1.0);

    if (material.has_diffuse_map)
        object_color *= texture(diffuse_map, UV);

    vec3 view_direction = normalize(Position - camera.position);

    vec3 light_strength = vec3(0.0);

    light_strength += calc_directional_lights(Normal, view_direction);
    light_strength += calc_point_lights(Normal, view_direction);
    light_strength += calc_spot_lights(Normal, view_direction);
    light_strength += calc_ambient_lights();

    out_color = vec4(light_strength * object_color.rgb, object_color.a);

    if (material.has_emission_map)
        out_color += texture(emission_map, UV);
}
