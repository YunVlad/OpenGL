#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "shader.h"

#include <vector>
#include <iostream>
#include <string>

class mesh
{

public:
    struct vertex
    {
        glm::vec3 pos;
        glm::vec3 norm;
        glm::vec2 tex;
    };

    struct texture
    {
        unsigned int id;
        std::string type;
        std::string path;
    };

    mesh(std::vector<vertex> vert, std::vector<unsigned int> ind, std::vector<texture> tex);
    void meshRender();
    void Draw(shader& shader);


private:
    unsigned VAO, VBO, EBO;
    std::vector<vertex> meshVert;
    std::vector<unsigned int> meshInd;
    std::vector<texture> meshTex;
};

#endif // !MESH_H