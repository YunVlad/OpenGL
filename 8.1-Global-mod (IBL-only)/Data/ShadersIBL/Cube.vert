#version 330 core

layout (location = 0) in vec3 Pos;

out vec3 currPos;

uniform mat4 view;
uniform mat4 proj;

void main()
{
    currPos = Pos;
    gl_Position = proj * view * vec4(Pos, 1.0);
};