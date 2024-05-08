#version 330 core

out vec3 color;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;

uniform vec3 albedoU;
uniform float metallic;
uniform float roughness;

uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;

uniform samplerCube shadowMap;

in vec3 currPos;
in vec3 norma;
in vec2 tex;

uniform vec3 viewPos;
uniform vec3 lightPos;

vec3 fresnelRoughness(vec3 h, vec3 v, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - max(dot(h, v), 0.0), 5.0);
}

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
     //shadow = 0;
     return shadow;
}
 
void main()
{
    vec3 albedo = pow(albedoU, vec3(2.2));
	vec3 norm = norma;
	vec3 viewDir = normalize(viewPos - currPos);
    vec3 refl = reflect(-viewDir, norm);

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);
    vec3 L0 = vec3(0.0);

    vec3 F = fresnelRoughness(norm, viewDir, F0, roughness);
    vec3 koefSpecular = F;
    vec3 koefDiffuse = vec3(1.0) - koefSpecular;
    koefDiffuse *= 1.0 - metallic;

    vec3 irradiance = texture(irradianceMap, norm).rgb;
    vec3 diffuse = irradiance * albedo;
    
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(prefilterMap, refl,  roughness * MAX_REFLECTION_LOD).rgb;    
    vec2 brdf  = texture(brdfLUT, vec2(max(dot(norm, viewDir), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    float shadow = addShadow(currPos, lightPos);
    vec3 ambient = koefDiffuse * diffuse + specular;
    color = (ambient + L0)*shadow;
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2)); 
};