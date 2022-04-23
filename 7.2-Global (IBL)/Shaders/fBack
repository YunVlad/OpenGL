#version 330 core

out vec4 color;

in vec3 currPos;

uniform samplerCube envMap;

void main()
{		
    vec3 envColor = texture(envMap, currPos).rgb;
   
    envColor = envColor / (envColor + vec3(1.0));
    envColor = pow(envColor, vec3(1.0/2.2));   
    color = vec4(envColor, 1.0);
}