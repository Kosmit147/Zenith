#version 460 core

in vec3 Position;
in vec3 Normal;
in vec2 UV;

layout (std140, binding = ZTH_CAMERA_UBO_BINDING_INDEX) uniform CameraUbo
{
	mat4 viewProjection;
    vec3 cameraPosition;
};

layout (std140, binding = ZTH_LIGHT_UBO_BINDING_INDEX) uniform LightUbo
{
    vec3 lightPosition;
	vec3 lightColor;

    vec3 lightAmbient;
    vec3 lightDiffuse;
    vec3 lightSpecular;
};

layout (std140, binding = ZTH_MATERIAL_UBO_BINDING_INDEX) uniform MaterialUbo
{
	vec3 albedo;
    bool has_diffuse_map;

    vec3 materialAmbient;
    vec3 materialDiffuse;
    vec3 materialSpecular;
    float shininess;
};

uniform sampler2D diffuseMap;

out vec4 outColor;

void main()
{
    vec4 objectColor = vec4(albedo, 1.0) * vec4(lightColor, 1.0);

    if (has_diffuse_map)
        objectColor *= texture(diffuseMap, UV);

    vec3 ambientFactor = lightAmbient * materialAmbient;

    vec3 lightDirection = normalize(lightPosition - Position);
    vec3 diffuseFactor = lightDiffuse * materialDiffuse * max(dot(lightDirection, Normal), 0.0);

    vec3 viewDirection = normalize(cameraPosition - Position);
    vec3 reflection = reflect(-lightDirection, Normal);
    vec3 specularFactor = lightSpecular * materialSpecular * pow(max(dot(reflection, viewDirection), 0.0), shininess);

    outColor = vec4((ambientFactor + diffuseFactor + specularFactor) * objectColor.rgb, objectColor.a);
}
