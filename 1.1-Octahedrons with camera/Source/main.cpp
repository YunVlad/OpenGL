#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "shader.h"
#include "camera.h"

camera Camera(glm::vec3(0.0f, -5.0f, -20.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, 0.0f);
float lastX = 640;
float lastY = 512;
bool firstMouse = true;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    Camera.ProcessMouseMovement(xoffset, yoffset);
}

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

    glm::vec3 currPos[] = 
    {   glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(5.0f,  5.0f, 5.0f),
        glm::vec3(0.0f, -3.0f, -3.0f),
        glm::vec3(0.0f, 0.0f, 7.0f),
        glm::vec3(-2.0f, 2.0f, -3.0f), };

    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(1280, 1024, "OpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

    while (!glfwWindowShouldClose(window))
    {
        float time = glfwGetTime();

        Camera.setCameraSpeed(time);

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            Camera.ProcessKeyboardMovement(FORWARD);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            Camera.ProcessKeyboardMovement(BACKWARD);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            Camera.ProcessKeyboardMovement(LEFT);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            Camera.ProcessKeyboardMovement(RIGHT);

        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 proj = glm::mat4(1.0f);
        proj = glm::perspective(glm::radians(45.0f), 1280.0f / 1024.0f, 0.1f, 100.0f);

        glm::mat4 model = glm::mat4(1.0f);

        glm::mat4 view = Camera.mat4getView();

        float color_sin = abs(sin(time));
        float color_cos = abs(cos(time));
        float color_ctg = color_cos / color_sin;

        testing.UseShaderProgramm();
        testing.mat4Uniform("proj", proj);
        testing.mat4Uniform("view", view);

        glBindVertexArray(VAO);
        for (int i = 0; i < 5; ++i)
        {
            model = glm::rotate(model, glm::radians(30.0f)*time, glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::translate(model, currPos[i]);

            testing.mat4Uniform("model", model);

            testing.vec4Uniform("Color", color_ctg/i, color_cos*i, color_sin+i, 1.0f);
            glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glfwTerminate();
    return 0;

}