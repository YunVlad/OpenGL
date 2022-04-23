#version 330 core

out vec4 color;

in vec3 currPos;

uniform samplerCube envMap;

void main()
{		
    vec3 envColor = texture(envMap, currPos).rgb;
    color = vec4(envColor, 1.0);
}