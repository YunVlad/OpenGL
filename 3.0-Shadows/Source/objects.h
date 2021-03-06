#ifndef OBJECTS_H
#define OBJECTS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>


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

    const float octahedron[] =

    { //Top
 0.0f,  0.0f,  1.0f,  1.0f, -1.0f, -1.0f,
 0.0f,  1.0f,  0.0f,  1.0f, -1.0f, -1.0f,
-1.0f,  0.0f,  0.0f,  1.0f, -1.0f, -1.0f,

 0.0f,  0.0f,  1.0f,  1.0f,  1.0f, -1.0f,
-1.0f,  0.0f,  0.0f,  1.0f,  1.0f, -1.0f,
 0.0f, -1.0f,  0.0f,  1.0f,  1.0f, -1.0f,

 0.0f,  0.0f,  1.0f, -1.0f,  1.0f, -1.0f,
 0.0f, -1.0f,  0.0f, -1.0f,  1.0f, -1.0f,
 1.0f,  0.0f,  0.0f, -1.0f,  1.0f, -1.0f,

 0.0f,  0.0f,  1.0f, -1.0f, -1.0f, -1.0f,
 1.0f,  0.0f,  0.0f, -1.0f, -1.0f, -1.0f,
 0.0f,  1.0f,  0.0f, -1.0f, -1.0f, -1.0f,

      //Bottom
 0.0f,  0.0f, -1.0f, -1.0f,  1.0f, -1.0f,
 0.0f,  1.0f,  0.0f, -1.0f,  1.0f, -1.0f,
-1.0f,  0.0f,  0.0f, -1.0f,  1.0f, -1.0f,

 0.0f,  0.0f, -1.0f, -1.0f, -1.0f, -1.0f,
-1.0f,  0.0f,  0.0f, -1.0f, -1.0f, -1.0f,
 0.0f, -1.0f,  0.0f, -1.0f, -1.0f, -1.0f,

 0.0f,  0.0f, -1.0f,  1.0f, -1.0f, -1.0f,
 0.0f, -1.0f,  0.0f,  1.0f, -1.0f, -1.0f,
 1.0f,  0.0f,  0.0f,  1.0f, -1.0f, -1.0f,

 0.0f,  0.0f, -1.0f,  1.0f,  1.0f, -1.0f,
 1.0f,  0.0f,  0.0f,  1.0f,  1.0f, -1.0f,
 0.0f,  1.0f,  0.0f,  1.0f,  1.0f, -1.0f,
    };

    std::vector<double> octahedron_v =

    {
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
      0.0f,  1.0f, 0.0f, };

    float cube[] = 
    {
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,

             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,

             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,

             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    
    float plate[] =

    {-1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
     -1.0f, 0.0f,-1.0f, 0.0f, 1.0f, 0.0f,
      1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,

     -1.0f,  0.0f,-1.0f, 0.0f, 1.0f, 0.0f,
      1.0f,  0.0f,-1.0f, 0.0f, 1.0f, 0.0f,
      1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, };

    //Materials
    glm::vec3 chrome_diff(0.4f, 0.4f, 0.4f);
    glm::vec3 chrome_spec(0.774597f, 0.774597f, 0.774597f);
    float chrome_shininess(0.6f);

    glm::vec3 obsidian_diff(0.18275f, 0.17f, 0.22525f);
    glm::vec3 obsidian_spec(0.332741f, 0.328634f, 0.346435f);
    float obsidian_shininess(0.3f);

    glm::vec3 bronze_diff(0.714f, 0.4284f, 0.18144f);
    glm::vec3 bronze_spec(0.393548f, 0.271906f, 0.166721f);
    float bronze_shininess(0.2f);

    glm::vec3 wrubber_diff(0.5f, 0.5f, 0.5f);
    glm::vec3 wrubber_spec(0.7f, 0.7f, 0.7f);
    float wrubber_shininess(0.078125f);
}

#endif // !OBJECTS_H
