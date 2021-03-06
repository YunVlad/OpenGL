#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "shader.h"
#include "camera.h"
#include "objects.h"

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

    glm::vec3 currPos[] = 
    {   glm::vec3(0.0f,  0.0f, 0.0f),
        glm::vec3(2.0f,  2.0f, 2.0f), };

    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(1280, 1024, "OpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    unsigned int VAO, VBO; 

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(objects::octahedron), objects::octahedron, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    shader shaders("Shaders/VertexCamera", "Shaders/objectColor");
    shader lightShaders("Shaders/VertexCamera", "Shaders/lightColor");

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
        //model = glm::rotate(model, glm::radians(30.0f) * time, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::translate(model, currPos[0]);

        glm::mat4 view = Camera.mat4getView();

        shaders.UseShaderProgramm();
        
        shaders.mat4Uniform("proj", proj);
        shaders.mat4Uniform("view", view);
        shaders.mat4Uniform("model", model);
        shaders.vec4Uniform("lightColor", 1.0f, 1.0f, 1.0f, 1.0f);
        shaders.vec4Uniform("objectColor", 1.0f, 0.0f, 0.0f, 1.0f);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 24);

        lightShaders.UseShaderProgramm();
        
        model = glm::mat4(1.0f);
        model = glm::translate(model, currPos[1]);
        model = glm::scale(model, glm::vec3(0.1f));
        lightShaders.mat4Uniform("model", model);
        lightShaders.mat4Uniform("proj", proj);
        lightShaders.mat4Uniform("view", view);

        glDrawArrays(GL_TRIANGLES, 0, 24);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glfwTerminate();
    return 0;

}