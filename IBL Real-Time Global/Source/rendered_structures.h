#pragma once

#include <glm/glm.hpp>

typedef struct 
{
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec3 atten;
    float diff;
    float spec;
} light_source;


typedef struct 
{
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
} material;
