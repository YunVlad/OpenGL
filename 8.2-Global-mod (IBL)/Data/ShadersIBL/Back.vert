#version 330 core

layout (location = 0) in vec3 Pos;

out vec3 currPos;

uniform mat4 view;
uniform mat4 proj;

void main()
{
    currPos = Pos;
    mat4 newView = mat4(mat3(view));
    vec4 cubePos = proj * newView * vec4(currPos, 1.0);

    gl_Position = cubePos.xyww;
};