#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "shader.h"

int main()
{
    float triangles[] =

    {-0.5f,  0.5f, 0.0f,
     -0.5f, -0.5f, 0.0f,
      0.5f, -0.5f, 0.0f,
      0.5f,  0.5f, 0.0f, 

      0.0f,  0.0f,-0.8f,
      0.0f,  0.0f, 0.8f, };



    unsigned int index[] =

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

    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(1280, 1024, "OpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    unsigned int VAO, VBO, EBO; 

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangles), triangles, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    shader testing("Shaders/VertexCamera", "Shaders/FragmentChangeColor");

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    float g_rot_x = 1.0f;
    float g_rot_y = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            g_rot_x = 1.0f;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            g_rot_x = -1.0f;
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            g_rot_y = 1.0f;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            g_rot_y = -1.0f;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            g_rot_x = 0.0f; g_rot_y = 0.0f;
        }
        glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float time = glfwGetTime();
        float color_sin = abs(sin(time));
        float color_cos = abs(cos(time));
        float color_ctg = color_cos / color_sin;

        glm::mat4 proj = glm::mat4(1.0f);
        proj = glm::perspective(glm::radians(45.0f), 1280.0f / 1024.0f, 0.1f, 100.0f);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));
        view = glm::rotate(view, (float)glfwGetTime(), glm::vec3(g_rot_x, g_rot_y, 0.0f));

        testing.UseShaderProgramm();
        testing.mat4Uniform("proj", proj);
        testing.mat4Uniform("model", model);
        testing.mat4Uniform("view", view);

        testing.vec4Uniform("Color", color_ctg, color_cos, color_sin, 1.0f);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;

}