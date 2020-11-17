#ifndef OBJECTS_H
#define OBJECTS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "shader.h"

namespace objects
{
    const float vert_triangles[] =

    {-0.5f,  0.5f, 0.0f,
     -0.5f, -0.5f, 0.0f,
      0.5f, -0.5f, 0.0f,
      0.5f,  0.5f, 0.0f,

      0.0f,  0.0f,-0.5f,
      0.0f,  0.0f, 0.5f, };

    const unsigned int vert_index[] =

    { 0, 2, 1,
      0, 2, 3,

      0, 1, 4,
      1, 2, 4,
      2, 3, 4,
      3, 0, 4,

      0, 1, 5,
      1, 2, 5,
      2, 3, 5,
      3, 0, 5, };

    float octahedron[] =

    { //Top
      0.0f,  0.0f, 1.0f, 
      0.0f,  1.0f, 0.0f,
     -1.0f,  0.0f, 0.0f,

      0.0f,  0.0f, 1.0f,
     -1.0f,  0.0f, 0.0f,
      0.0f, -1.0f, 0.0f,

      0.0f,  0.0f, 1.0f,
      0.0f, -1.0f, 0.0f,
      1.0f,  0.0f, 0.0f,

      0.0f,  0.0f, 1.0f,
      1.0f,  0.0f, 0.0f,
      0.0f,  1.0f, 0.0f,

      //Bottom
      0.0f,  0.0f,-1.0f,
      0.0f,  1.0f, 0.0f,
     -1.0f,  0.0f, 0.0f,

      0.0f,  0.0f,-1.0f,
     -1.0f,  0.0f, 0.0f,
      0.0f, -1.0f, 0.0f,

      0.0f,  0.0f,-1.0f,
      0.0f, -1.0f, 0.0f,
      1.0f,  0.0f, 0.0f,

      0.0f,  0.0f,-1.0f,
      1.0f,  0.0f, 0.0f,
      0.0f,  1.0f, 0.0f,
    };
}

#endif // !OBJECTS_H
