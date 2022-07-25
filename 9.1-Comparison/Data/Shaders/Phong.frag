#version 330 core

out vec3 color;

uniform samplerCube shadowMap;

struct pointLight 
{
    vec3 pos;
    vec3 color;
    vec3 atten;
    float diff;
    float spec;
};

struct materialType
{
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

#define NR_LIGHTS 1

in vec3 norma;
in vec3 currPos;

uniform vec3 viewPos;
uniform materialType material;
uniform pointLight lights[NR_LIGHTS];

float addShadow(vec3 currPos, vec3 lightPos)
{
    vec3 fragToLight = currPos - lightPos; 
    float currentDepth = length(fragToLight);
    float shadow = 0.0;
    float bias = 0.05; 
    float samples = 4.0;
    float offset = 0.1;

    for (float x = -offset; x < offset; x += offset / (samples * 0.5))
    {
         for(float y = -offset; y < offset; y += offset / (samples * 0.5))
         {
             for(float z = -offset; z < offset; z += offset / (samples * 0.5))
             {
                 float closestDepth = texture(shadowMap, fragToLight + vec3(x, y, z)).r;
                 closestDepth *= 100.0; 
                 if(currentDepth - bias > closestDepth)
                     shadow += 1.0;
             }
         }
     }
     shadow /= (samples * samples * samples);
     return shadow;
}

vec3 addPointLight(pointLight light, vec3 norm, vec3 currPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.pos - currPos);
    vec3 reflectDir = reflect(-lightDir, norm); 

    float diff = max(dot(norm, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128*material.shininess);

    float distance = length(light.pos - currPos);
    float attenuation = 1.0 / (light.atten.x + light.atten.y * distance + light.atten.z * (distance * distance));

    vec3 diffuseLight = diff * light.diff * light.color * material.diffuse;
    vec3 specularLight = spec * light.spec * light.color * material.specular;

    float shadow = addShadow(currPos, light.pos);

    return (diffuseLight + specularLight)*attenuation*(1.0 - shadow);
}
 
void main()
{

	vec3 norm = normalize(norma);
	vec3 viewDir = normalize(viewPos - currPos);

    color = addPointLight(lights[0], norm, currPos, viewDir);
    for (int i = 1; i < NR_LIGHTS; i++)
        color += addPointLight(lights[i], norm, currPos, viewDir);

};