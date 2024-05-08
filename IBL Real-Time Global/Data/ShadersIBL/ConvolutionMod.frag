#version 330 core

out vec4 color;

in vec3 currPos;

uniform samplerCube envMap;

void main()
{	
    float a[4] = float[4](0.1012285, 0.2223810, 0.3137066, 0.3626837);
    float ct[4] = float[4](0.9602898, 0.7966664, 0.5255324, 0.1834346);
    float st[4] = float[4](0.2790044, 0.6044192, 0.8507736, 0.9830319);
    float PI = 3.14159;
    vec3 res = vec3(0.0);  
    float fi, cos_fi, sin_fi, rs, rc;
    float PI8 = PI / 8.0;

    vec3 norm = normalize(currPos);
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(up, norm);
    up = cross(norm, right);

    vec3 irradiance = vec3(0.0);          

    for (int i = 0; i < 16; ++i)
    {
        fi = PI8 * i;
        cos_fi = cos(fi);
        sin_fi = sin(fi);
        for (int j = 0; j < 4; ++j)
        {
            vec3 tangentSample = vec3(st[j] * cos_fi, st[j] * sin_fi,  ct[j]);
            vec3 sampleVec1 = tangentSample.x * right + tangentSample.y * up + tangentSample.z * norm; 
            vec3 sampleVec2 = tangentSample.x * right + tangentSample.y * up - tangentSample.z * norm; 

            irradiance += a[j]*(texture(envMap, sampleVec1).rgb);
        }      
    }

    irradiance = PI8 * irradiance;
    color = vec4(irradiance, 1.0);
}