#version 330 core

layout (location = 0) in vec3 Pos;
layout (location = 1) in vec3 nPos;


out vec3 currPos;
out vec3 norma;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    norma = nPos;
    currPos = vec3(model * vec4(Pos, 1.0));

    gl_Position = proj * view * model * vec4(Pos, 1.0);
};