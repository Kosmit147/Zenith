#version 460 core

struct Material
{
    vec3 albedo;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

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

//! #define ZTH_CAMERA_UBO_BINDING_INDEX 0
//! #define ZTH_LIGHT_UBO_BINDING_INDEX 1
//! #define ZTH_MATERIAL_UBO_BINDING_INDEX 2

layout (std140, binding = ZTH_CAMERA_UBO_BINDING_INDEX) uniform CameraUbo
{
    mat4 viewProjection;
    vec3 cameraPosition;
};

layout (std140, binding = ZTH_LIGHT_UBO_BINDING_INDEX) uniform LightUbo
{
    vec3 directionalLightDirection;
    LightProperties directionalLightProperties;

    vec3 pointLightPosition;
    LightProperties pointLightProperties;
    LightAttenuation pointLightAttenuation;

    bool hasDirectionalLight;
    bool hasPointLight;
};

layout (std140, binding = ZTH_MATERIAL_UBO_BINDING_INDEX) uniform MaterialUbo
{
    Material material;

    bool hasDiffuseMap;
    bool hasSpecularMap;
    bool hasEmissionMap;
};

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D emissionMap;

out vec4 outColor;

float getAttenuation(LightAttenuation attenuation, float dist)
{
    return 1.0 / (attenuation.constant + attenuation.linear * dist + attenuation.quadratic * (dist * dist));
}

Light createPointLight()
{
    vec3 diff = Position - pointLightPosition;
    float dist = length(diff);
    float attenuation = getAttenuation(pointLightAttenuation, dist);
    vec3 pointLightDirection = normalize(diff);

    return Light(pointLightDirection, pointLightProperties, attenuation);
}

Light createDirectionalLight()
{
    return Light(normalize(directionalLightDirection), directionalLightProperties, 1.0);
}

vec3 getAmbientStrength(Light light)
{
    return light.properties.ambient;
}

vec3 getDiffuseStrength(Light light)
{
    return light.properties.diffuse * max(dot(-light.direction, Normal), 0.0);
}

vec3 getSpecularStrength(Light light)
{
    vec3 viewDirection = normalize(Position - cameraPosition);
    vec3 reflection = reflect(light.direction, Normal);
    vec3 specularFactor = light.properties.specular * pow(max(dot(reflection, -viewDirection), 0.0), material.shininess);

    if (hasSpecularMap)
        specularFactor *= vec3(texture(specularMap, UV));

    return specularFactor;
}

vec3 getLightStrength(Light light)
{
    vec3 ambient = getAmbientStrength(light) * material.ambient;
    vec3 diffuse = getDiffuseStrength(light) * material.diffuse;
    vec3 specular = getSpecularStrength(light) * material.specular;
    return (ambient + diffuse + specular) * light.properties.color * light.attenuation;
}

void main()
{
    vec4 objectColor = vec4(material.albedo, 1.0);

    if (hasDiffuseMap)
        objectColor *= texture(diffuseMap, UV);

    vec3 lightStrength = vec3(0.0);

    if (hasPointLight)
    {
        Light pointLight = createPointLight();
        lightStrength += getLightStrength(pointLight);
    }

    if (hasDirectionalLight)
    {
        Light directionalLight = createDirectionalLight();
        lightStrength += getLightStrength(directionalLight);
    }

    outColor = vec4(lightStrength * objectColor.rgb, objectColor.a);

    if (hasEmissionMap)
        outColor += texture(emissionMap, UV);
}
