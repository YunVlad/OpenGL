#version 330 core

out vec4 color;

in vec3 currPos;

uniform samplerCube envMap;
uniform float roughness;

const float PI = 3.14159;

float VanDerCorpus(uint n, uint base)
{
    float invBase = 1.0 / float(base);
    float den = 1.0;
    float result = 0.0;
 
    for (uint i = 0u; i < 32u; ++i)
    {
        if(n > 0u)
        {
            den = mod(float(n), 2.0);
            result += den * invBase;
            invBase = invBase / 2.0;
            n = uint(float(n) / 2.0);
        }
    }
 
    return result;
}

vec2 Hammersley(uint i, uint N)
{
    return vec2(float(i)/float(N), VanDerCorpus(i, 2u));
}

float TrowbridgeReitzGGX(vec3 n, vec3 h, float roughness)
{
    float num = pow(roughness, 4.0);
    float nh = pow(max(dot(n, h), 0.0), 2.0);
    float den = 3.14159 * pow( nh*(num - 1.0) + 1.0, 2.0);

    return num/den;
}

vec3 SampleGGX(vec2 xi, vec3 n, float roughness)
{
	float a = pow(roughness, 2.0);
	
	float phi = 2.0 * PI * xi.x;
	float cosTheta = sqrt((1.0 - xi.y) / (1.0 + (a*a - 1.0) * xi.y));
	float sinTheta = sqrt(1.0 - cosTheta*cosTheta);
	
	vec3 h;
	h.x = cos(phi) * sinTheta;
	h.y = sin(phi) * sinTheta;
	h.z = cosTheta;
	
	vec3 up = abs(n.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
	vec3 tangent   = normalize(cross(up, n));
	vec3 bitangent = cross(n, tangent);
	
	vec3 sample = tangent * h.x + bitangent * h.y + n * h.z;
	return normalize(sample);
}
 
void main()
{
    vec3 N = normalize(currPos);
    vec3 R = N;
    vec3 V = N;

    const uint SAMPLE_COUNT = 1024u;
    vec3 prefilteredColor = vec3(0.0);
    float weight = 0.0;

	for(uint i = 0u; i < SAMPLE_COUNT; ++i)
    {
	    vec2 phi = Hammersley(i, SAMPLE_COUNT);
        vec3 H = SampleGGX(phi, N, roughness);
        vec3 L  = normalize(2.0 * dot(V, H) * H - V);

        float NdotL = max(dot(N, L), 0.0);
        if(NdotL > 0.0)
        {
            float D = TrowbridgeReitzGGX(N, H, roughness);
            float NdotH = max(dot(N, H), 0.0);
            float HdotV = max(dot(H, V), 0.0);
            float pdf = D * NdotH / (4.0 * HdotV) + 0.0001; 

            float resolution = 512.0;
            float saTexel  = 4.0 * PI / (6.0 * resolution * resolution);
            float saSample = 1.0 / (float(SAMPLE_COUNT) * pdf + 0.0001);

            float mipLevel = roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel); 
            
            prefilteredColor += textureLod(envMap, L, mipLevel).rgb * NdotL;
            weight += NdotL;
        }
    }

    prefilteredColor = prefilteredColor / weight;
    color = vec4(prefilteredColor, 1.0);

};