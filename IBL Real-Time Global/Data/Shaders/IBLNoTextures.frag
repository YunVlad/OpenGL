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

in vec3 currPos;
in vec3 norma;
in vec2 tex;

uniform vec3 viewPos;

vec3 fresnelRoughness(vec3 h, vec3 v, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - max(dot(h, v), 0.0), 5.0);
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

    vec3 ambient = koefDiffuse * diffuse + specular;
    color = ambient + L0;
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2)); 
};