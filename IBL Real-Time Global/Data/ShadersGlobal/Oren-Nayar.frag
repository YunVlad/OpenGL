#version 330 core

out vec3 color;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;

uniform samplerCube shadowMap;

struct pointLight 
{
    vec3 pos;
    vec3 color;
    vec3 atten;
    float diff;
    float spec;
};

#define NR_LIGHTS 3

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
    vec3 N   = normalize(norma);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tanNormal);
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
     return shadow;
}

vec3 Fresnel(vec3 h, vec3 v, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - max(dot(h, v), 0.0), 5.0);
}

vec3 RenderingEquation(pointLight light, vec3 normal, vec3 currPos, vec3 viewDir, float roughness, float metallic, vec3 albedo)
{
    vec3 lightDir = normalize(light.pos - currPos);
    vec3 H = normalize(viewDir + lightDir);

    float distance = length(light.pos - currPos);
    float attenuation = 1.0 / (light.atten.x + light.atten.y * distance + light.atten.z * (distance * distance));
    vec3 radiance = light.color * attenuation;

    float sigma = roughness * roughness;
    float PI_rev = 1 / 3.14159;
    float NdotL = dot(normal, lightDir);
    float NdotV = dot(normal, viewDir);

    //float a = PI_rev * (1.0 - (0.5 * (sigma / (sigma + 0.33))) + (0.17 * albedo * (sigma / (sigma + 0.13))));
    float a = PI_rev * (1.0 - (0.5 * (sigma / (sigma + 0.33))) + (0.17 * (sigma / (sigma + 0.13))));
    float b = PI_rev * (0.45 * (sigma / (sigma + 0.09)));
    float s = dot(lightDir, viewDir) - (NdotL * NdotV);

    float t = 1.0;
    if (s > 0)
        t = max(NdotL, NdotV);

    float lambert = max(NdotL, 0.0) * (a + (b * (s / t)));

    //vec3 F0 = vec3(0.01); 
    //F0 = mix(F0, albedo, metallic);
    //float specularity = F0.x + (1.0 - F0.x) * pow(1.0 - max(dot(H, viewDir), 0.0), 5.0);
    float specularity = (0.001)*metallic;

    /*float specularity = 0.0;
    if (lambert > 0) 
    {
        float specularAngle = max(dot(H, normal), 0.0);
        //specularity = pow(specularAngle, metallic);
        specularity = pow(1.0 - metallic, 5.0);
    }*/ 

    vec3 L = (lambert * albedo + specularity) * radiance;
    //float shadow = (1.0 - addShadow(currPos, light.pos)) * attenuation;
    //vec3 L = lambert * albedo * radiance;
    return L;
    //return L*shadow;
    //return albedo;
}
 
void main()
{
    vec3 albedo = texture(albedoMap, tex).rgb;
    float metallic = texture(metallicMap, tex).r;
    float roughness = texture(roughnessMap, tex).r;
	vec3 norm = getNormal();
	vec3 viewDir = normalize(viewPos - currPos);


    vec3 L0 = vec3(0.0);
    for (int i = 0; i < NR_LIGHTS; i++)
        L0 += RenderingEquation(lights[i], norm, currPos, viewDir, roughness, metallic, albedo);

    vec3 ambient = vec3(0.001) * albedo;   

    color = (ambient + L0);
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2)); 
}