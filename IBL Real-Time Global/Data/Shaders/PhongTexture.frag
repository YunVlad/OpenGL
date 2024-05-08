#version 330 core

out vec3 color;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;

in vec3 norma;
in vec3 currPos;
in vec2 tex;

struct pointLight 
{
    vec3 pos;
    vec3 color;
    vec3 atten;
    float diff;
    float spec;
};

#define NR_LIGHTS 1
uniform vec3 viewPos;
uniform pointLight lights[NR_LIGHTS];
 
void main()
{
    float ambientStrength = 0.1;
    vec3 ambientLight = ambientStrength * lights[0].color;
    vec3 albedo = texture(albedoMap, tex).rgb * 0.3;

    vec3 norm = normalize(norma);
    vec3 lightDir = normalize(lights[0].pos - currPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuseLight = diff * lights[0].color;

    float specularStrength = 0.7;
    vec3 viewDir = normalize(viewPos - currPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    float specular = pow(max(dot(viewDir, reflectDir), 0.0), 64);
    vec3 specularLight = specularStrength * specular * lights[0].color;

    float distance = length(lights[0].pos - currPos);
    float attenuation = 1.0 / (lights[0].atten.x + lights[0].atten.y * distance + lights[0].atten.z * (distance * distance));

    color = (albedo +  (ambientLight * diffuseLight + specularLight))*attenuation;
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2)); 
};