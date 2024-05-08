#version 330 core

out vec3 color;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;

in vec3 norma;
in vec3 currPos;
in vec2 tex;
  
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
 
void main()
{
    float ambientStrength = 0.2;
    vec3 ambientLight = ambientStrength * lightColor;
    vec3 albedo = texture(albedoMap, tex).rgb;

    vec3 norm = normalize(norma);
    vec3 lightDir = normalize(lightPos - currPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuseLight = diff * lightColor;

    float specularStrength = 0.9;
    vec3 viewDir = normalize(viewPos - currPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    float specular = pow(max(dot(viewDir, reflectDir), 0.0), 64);
    vec3 specularLight = specularStrength * specular * lightColor;

    color = albedo * 0.5 *  (ambientLight + diffuseLight + specularLight);
};