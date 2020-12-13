#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "shader.h"
#include "camera.h"
#include "objects.h"


camera Camera(glm::vec3(5.5f, -4.5f, -8.7f), glm::vec3(0.0f, 1.0f, 0.0f), 120.0f, 20.0f);
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
    { glm::vec3(0.0f,  0.0f, 0.0f),
      glm::vec3(-2.0f,  -2.0f, -2.0f), };

    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(1920, 1080, "OpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    unsigned int VBO, VBO_light, VAO, VAO_light;

    glGenVertexArrays(1, &VAO);
    glGenVertexArrays(1, &VAO_light);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VBO_light);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(objects::cube), objects::cube, GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(VAO_light);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_light);
    glBufferData(GL_ARRAY_BUFFER, sizeof(objects::octahedron), objects::octahedron, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    shader shaders("Shaders/VertexObject", "Shaders/objectColor");
    shader lightShaders("Shaders/VertexStand", "Shaders/lightColor");


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

        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 proj = glm::mat4(1.0f);
        proj = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, currPos[0]);

        glm::mat4 view = Camera.mat4getView();

        glm::vec3 lightPos = currPos[1];
        lightPos.x = 3*sin(time);
        lightPos.y = 5*cos(time);
        glm::vec3 viewPos = Camera.getCameraPos();
        //std::cout << viewPos.x << " " << viewPos.y << " " << viewPos.z << std::endl;

        shaders.UseShaderProgramm();

        shaders.mat4Uniform("proj", proj);
        shaders.mat4Uniform("view", view);
        shaders.mat4Uniform("model", model);
        shaders.vec3Uniform("lightColor", 1.0f, 1.0f, 1.0f);
        shaders.vec3Uniform("lightPos", lightPos);
        shaders.vec3Uniform("viewPos", viewPos);
        shaders.vec3Uniform("objectColor", 0.0f, 1.0f, 0.0f);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        lightShaders.UseShaderProgramm();

        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.1f));
        lightShaders.mat4Uniform("model", model);
        lightShaders.mat4Uniform("proj", proj);
        lightShaders.mat4Uniform("view", view);

        glBindVertexArray(VAO_light);
        glDrawArrays(GL_TRIANGLES, 0, 24);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glfwTerminate();
    return 0;

}