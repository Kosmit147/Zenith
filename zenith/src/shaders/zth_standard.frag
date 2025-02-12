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

struct PointLight
{
    vec3 position;
    LightProperties properties;
    LightAttenuation attenuation;
};

in vec3 Position;
in vec3 Normal;
in vec2 UV;

layout (std140, binding = ZTH_CAMERA_UBO_BINDING_POINT) uniform CameraUbo
{
    mat4 view_projection;
    vec3 position;
} camera;

layout (std430, binding = ZTH_LIGHT_SSBO_BINDING_POINT) restrict readonly buffer LightSsbo
{
    vec3 directional_light_direction;
    LightProperties directional_light_properties;

    vec3 spot_light_position;
    vec3 spot_light_direction;
    float spot_light_inner_cutoff;
    float spot_light_outer_cutoff;
    LightProperties spot_light_properties;
    LightAttenuation spot_light_attenuation;

    bool has_directional_light;
    bool has_spot_light;
} light;

layout (std430, binding = ZTH_POINT_LIGHTS_SSBO_BINDING_POINT) restrict readonly buffer PointLightsSsbo
{
    uint count;
    PointLight lights[];
} point_lights;

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

uniform sampler2D diffuse_map;
uniform sampler2D specular_map;
uniform sampler2D emission_map;

out vec4 out_color;

float calc_strength(LightAttenuation attenuation, float dist)
{
    return 1.0 / (attenuation.constant + attenuation.linear * dist + attenuation.quadratic * (dist * dist));
}

Light convert_directional_light()
{
    return Light(normalize(light.directional_light_direction), light.directional_light_properties, 1.0);
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
Light convert_spot_light(float dist, float angle)
{
    float strength = calc_strength(light.spot_light_attenuation, dist);

    float intensity = (angle - light.spot_light_outer_cutoff) / (light.spot_light_inner_cutoff - light.spot_light_outer_cutoff);
    intensity = clamp(intensity, 0.0, 1.0);
    strength *= intensity;

    return Light(normalize(light.spot_light_direction), light.spot_light_properties, strength);
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

    if (light.has_directional_light)
    {
        Light directional_light = convert_directional_light();
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

    if (light.has_spot_light)
    {
	    vec3 diff_from_spot_light = Position - light.spot_light_position;
	    float distance_from_spot_light = length(diff_from_spot_light);
        vec3 normalized_diff = diff_from_spot_light / distance_from_spot_light;
	    float angle = dot(light.spot_light_direction, normalized_diff);

        if (angle > light.spot_light_outer_cutoff)
        {
            Light spot_light = convert_spot_light(distance_from_spot_light, angle);
            result += calc_light(spot_light, normal, view_direction);
        }
    }

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

    out_color = vec4(light_strength * object_color.rgb, object_color.a);

    if (material.has_emission_map)
        out_color += texture(emission_map, UV);
}
