#version 330 core

out vec4 color;

in vec3 currPos;

uniform samplerCube envMap;

void main()
{	
    float PI = 3.14159;
    vec3 norm = normalize(currPos);
    vec3 irradiance = vec3(0.0);   
    
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(up, norm);
    up = cross(norm, right);
       
    float delta = 0.025;
    float nrSamples = 0.0f;

    for(float phi = 0.0; phi < 2.0 * PI; phi += delta)
    {
        for(float theta = 0.0; theta < 0.5 * PI; theta += delta)
        {
            vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * norm; 
            irradiance += texture(envMap, sampleVec).rgb * cos(theta) * sin(theta);
            nrSamples++;
        }
    }
    irradiance = PI * irradiance * (1.0 / float(nrSamples));
    color = vec4(irradiance, 1.0);
}