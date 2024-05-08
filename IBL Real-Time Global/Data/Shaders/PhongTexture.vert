#version 330 core

layout (location = 0) in vec3 Pos;
layout (location = 1) in vec3 nPos;
layout (location = 2) in vec2 tPos;


out vec3 norma;
out vec3 currPos;
out vec2 tex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
 
void main()
{
    norma = nPos;
    currPos = vec3(model * vec4(Pos, 1.0));
    tex = tPos;
    
    gl_Position = proj * view * vec4(currPos, 1.0);
};