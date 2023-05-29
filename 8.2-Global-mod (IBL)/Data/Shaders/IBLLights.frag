#version 330 core

out vec3 color;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;

uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;

struct pointLight 
{
    vec3 pos;
    vec3 color;
    vec3 atten;
    float diff;
    float spec;
};

#define NR_LIGHTS 1

in vec3 norma;
in vec3 currPos;
in vec2 tex;

uniform vec3 viewPos;
uniform pointLight lights[NR_LIGHTS];

vec3 getNormal()
{
    vec3 tanNormal = texture(normalMap, tex).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(currPos);
    vec3 Q2  = dFdy(currPos);
    vec2 st1 = dFdx(tex);
    vec2 st2 = dFdy(tex);

    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 N  = normalize(norma);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tanNormal);
}

float TrowbridgeReitzGGX(vec3 n, vec3 h, float roughness)
{
    float num = pow(roughness, 4.0);
    float nh = pow(max(dot(n, h), 0.0), 2.0);
    float den = 3.14159 * pow( nh*(num - 1.0) + 1.0, 2.0);

    return num/den;
}

float SchlickGGX(vec3 n, vec3 v, float roughness)
{
    float r = (roughness + 1.0);
    float k = pow(r, 2.0) / 8.0;

    float num   = max(dot(n, v), 0.0);
    float den = num * (1.0 - k) + k;

    return num/den;
}

float GeometrySmith(vec3 n, vec3 v, vec3 L, float roughness)
{
    return (SchlickGGX(n, v, roughness) * SchlickGGX(n, L, roughness));
}

vec3 Fresnel(vec3 h, vec3 v, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - max(dot(h, v), 0.0), 5.0);
}

vec3 fresnelRoughness(vec3 h, vec3 v, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - max(dot(h, v), 0.0), 5.0);
}

vec3 RenderingEquation(pointLight light, vec3 Norm, vec3 currPos, vec3 viewDir, float roughness, float metallic, vec3 albedo, vec3 F0)
{
    vec3 LightDir = normalize(light.pos - currPos);
    vec3 H = normalize(viewDir + LightDir);

    float distance = length(light.pos - currPos);
    float attenuation = 1.0 / (light.atten.x + light.atten.y * distance + light.atten.z * (distance * distance));
    vec3 radiance = light.color * attenuation;

    float NDF = TrowbridgeReitzGGX(Norm, H, roughness);   
    float G = GeometrySmith(Norm, viewDir, LightDir, roughness);   
    vec3 F = Fresnel(H, viewDir, F0);

    vec3 num = NDF * G * F; 
    float den = 4 * max(dot(Norm, viewDir), 0.0) * max(dot(Norm, LightDir), 0.0) + 0.001;
    vec3 specular = num/den;

    vec3 koefSpecular = F;
    vec3 koefDiffuse = vec3(1.0) - koefSpecular;
    koefDiffuse *= 1.0 - metallic;	  
    float NL = max(dot(Norm, LightDir), 0.0);        

    vec3 L = (koefDiffuse * albedo / 3.14159 + specular) * radiance * NL;
    return L;
}
 
void main()
{
    vec3 albedo = pow(texture(albedoMap, tex).rgb, vec3(2.2));
    float metallic = texture(metallicMap, tex).r;
    float roughness = texture(roughnessMap, tex).r;

	vec3 norm = getNormal();
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

    for (int i = 0; i < NR_LIGHTS; i++)
        L0 += RenderingEquation(lights[i], norm, currPos, viewDir, roughness, metallic, albedo, F0);

    vec3 ambient = koefDiffuse * diffuse + specular;
    color = ambient + L0;
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2)); 
};