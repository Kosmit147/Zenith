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
    float attenuation;
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

    vec3 point_light_position;
    LightProperties point_light_properties;
    LightAttenuation point_light_attenuation;

    vec3 spot_light_position;
    vec3 spot_light_direction;
    float spot_light_inner_cutoff;
    float spot_light_outer_cutoff;
    LightProperties spot_light_properties;
    LightAttenuation spot_light_attenuation;

    bool has_directional_light;
    bool has_point_light;
    bool has_spot_light;
} light;

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

float get_attenuation(LightAttenuation attenuation, float dist)
{
    return 1.0 / (attenuation.constant + attenuation.linear * dist + attenuation.quadratic * (dist * dist));
}

Light create_directional_light()
{
    return Light(normalize(light.directional_light_direction), light.directional_light_properties, 1.0);
}

Light create_point_light()
{
    vec3 diff = Position - light.point_light_position;
    float dist = length(diff);
    float attenuation = get_attenuation(light.point_light_attenuation, dist);
    vec3 point_light_direction = normalize(diff);

    return Light(point_light_direction, light.point_light_properties, attenuation);
}

// dist is the distance from frag position to spot light
// angle is the dot product of the direction from spot light to frag position and spot light direction
Light create_spot_light(float dist, float angle)
{
    float attenuation = get_attenuation(light.spot_light_attenuation, dist);

    float intensity = (angle - light.spot_light_outer_cutoff) / (light.spot_light_inner_cutoff - light.spot_light_outer_cutoff);
    intensity = clamp(intensity, 0.0, 1.0);
    attenuation *= intensity;

    return Light(normalize(light.spot_light_direction), light.spot_light_properties, attenuation);
}

vec3 get_ambient_strength(Light light)
{
    return light.properties.ambient;
}

vec3 get_diffuse_strength(Light light, vec3 normal)
{
    return light.properties.diffuse * max(dot(-light.direction, normal), 0.0);
}

vec3 get_specular_strength(Light light, vec3 normal, vec3 view_direction)
{
    vec3 reflection = reflect(light.direction, normal);
    vec3 specular_factor = light.properties.specular * pow(max(dot(reflection, -view_direction), 0.0), material.shininess);

    if (material.has_specular_map)
        specular_factor *= vec3(texture(specular_map, UV));

    return specular_factor;
}

vec3 get_light_strength(Light light, vec3 normal, vec3 view_direction)
{
    vec3 ambient = get_ambient_strength(light) * material.ambient;
    vec3 diffuse = get_diffuse_strength(light, normal) * material.diffuse;
    vec3 specular = get_specular_strength(light, normal, view_direction) * material.specular;
    return (ambient + diffuse + specular) * light.properties.color * light.attenuation;
}

vec3 calc_directional_light(vec3 normal, vec3 view_direction)
{
    vec3 result = vec3(0.0);

    if (light.has_directional_light)
    {
        Light directional_light = create_directional_light();
        result += get_light_strength(directional_light, normal, view_direction);
    }

    return result;
}

vec3 calc_point_light(vec3 normal, vec3 view_direction)
{
    vec3 result = vec3(0.0);

    if (light.has_point_light)
    {
        Light point_light = create_point_light();
        result += get_light_strength(point_light, normal, view_direction);
    }

    return result;
}

vec3 calc_spot_light(vec3 normal, vec3 view_direction)
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
            Light spot_light = create_spot_light(distance_from_spot_light, angle);
            result += get_light_strength(spot_light, normal, view_direction);
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

    light_strength += calc_directional_light(Normal, view_direction);
    light_strength += calc_point_light(Normal, view_direction);
    light_strength += calc_spot_light(Normal, view_direction);

    out_color = vec4(light_strength * object_color.rgb, object_color.a);

    if (material.has_emission_map)
        out_color += texture(emission_map, UV);
}
