#version 330 core

in vec4 currPos;

uniform vec3 lightPos;
uniform float far_plane;

void main()
{
    float lightDistance = length(currPos.xyz - lightPos);
    
    lightDistance = lightDistance / far_plane;

    gl_FragDepth = lightDistance;
}