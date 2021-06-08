#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "shader.h"
#include "camera.h"
#include "objectsRender.h"


camera Camera(glm::vec3(-0.5f, 10.0f, -9.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, -45.0f);
float lastX = 960;
float lastY = 540;
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
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(1920, 1080, "OpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);

    objectsRender render;
    render.sphereInit(32);
    unsigned int ventedTexture1 = render.loadTexture("PBR/vented/albedo.jpg");
    unsigned int ventedTexture2 = render.loadTexture("PBR/vented/ao.jpg");
    unsigned int ventedTexture3 = render.loadTexture("PBR/vented/normal.jpg");
    unsigned int ventedTexture4 = render.loadTexture("PBR/vented/metallic.jpg");
    unsigned int ventedTexture5 = render.loadTexture("PBR/vented/roughness.jpg");

    unsigned int ceramicTexture1 = render.loadTexture("PBR/greenCeramic/albedo.jpg");
    unsigned int ceramicTexture2 = render.loadTexture("PBR/greenCeramic/ao.jpg");
    unsigned int ceramicTexture3 = render.loadTexture("PBR/greenCeramic/normal.jpg");
    unsigned int ceramicTexture4 = render.loadTexture("PBR/greenCeramic/metallic.jpg");
    unsigned int ceramicTexture5 = render.loadTexture("PBR/greenCeramic/roughness.jpg");

    unsigned int beatenTexture1 = render.loadTexture("PBR/beaten/albedo.jpg");
    unsigned int beatenTexture2 = render.loadTexture("PBR/beaten/ao.jpg");
    unsigned int beatenTexture3 = render.loadTexture("PBR/beaten/normal.jpg");
    unsigned int beatenTexture4 = render.loadTexture("PBR/beaten/metallic.jpg");
    unsigned int beatenTexture5 = render.loadTexture("PBR/beaten/roughness.jpg");

    //Shaders configure

    shader lightShaders("Shaders/VertexLight", "Shaders/ColorLight");

    shader PBRshaders("Shaders/VertexPBR", "Shaders/ColorPBR2");
    shader Normalshaders("Shaders/VertexNormal", "Shaders/ColorNormal2");
    shader shaders("Shaders/VertexTexture", "Shaders/ColorTexture2");


    glm::vec3 lightPos1 = glm::vec3(0.0f, 5.0f, 0.0f);
    glm::vec3 lightColor1 = glm::vec3(1.0f, 1.0f, 1.0f);
    int currLight = 1;

    glm::vec3 currPos[] =
    { glm::vec3(0.0f,  0.0f, 0.0f),
      glm::vec3(0.0f,  0.0f, 7.0f),
      glm::vec3(2.0f,  0.0f, 0.0f),
      glm::vec3(7.0f,  0.0f, 0.0f), };

    PBRshaders.UseShaderProgramm();
    PBRshaders.IntUniform("albedoMap", 1);
    PBRshaders.IntUniform("aoMap", 2);
    PBRshaders.IntUniform("normalMap", 3);
    PBRshaders.IntUniform("metallicMap", 4);
    PBRshaders.IntUniform("roughnessMap", 5);

    Normalshaders.UseShaderProgramm();
    Normalshaders.IntUniform("textureMap", 1);
    Normalshaders.IntUniform("normalMap", 2);

    shaders.UseShaderProgramm();
    shaders.IntUniform("textureMap", 1);


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

        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
            currLight = 1;

        if ((glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) && (currLight == 1))
            lightPos1.z += 0.1f;
        if ((glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) && (currLight == 1))
            lightPos1.z -= 0.1f;
        if ((glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) && (currLight == 1))
            lightPos1.x += 0.1f;
        if ((glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) && (currLight == 1))
            lightPos1.x -= 0.1f;
        if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) && (currLight == 1))
            lightPos1.y += 0.1f;
        if ((glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) && (currLight == 1))
            lightPos1.y -= 0.1f;

        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
            lightColor1 = glm::vec3(1.0f, 0.1f, 0.1f);
        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
            lightColor1 = glm::vec3(0.0f, 1.0f, 0.0f);
        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
            lightColor1 = glm::vec3(0.0f, 0.0f, 1.0f);
        if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
            lightColor1 = glm::vec3(1.0f, 1.0f, 1.0f);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glViewport(0, 0, 1920, 1080);

        //Camera and projective matrixes

        glm::mat4 proj = glm::mat4(1.0f);
        proj = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);
        glm::mat4 view = Camera.mat4getView();
        glm::vec3 viewPos = Camera.getCameraPos();


        // Main render
        // PBR

        PBRshaders.UseShaderProgramm();

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
        glm::vec3 Pos = glm::vec3(currPos[0].x, currPos[0].y, currPos[0].z);
        model = glm::translate(model, Pos);

        PBRshaders.mat4Uniform("proj", proj);
        PBRshaders.mat4Uniform("view", view);
        PBRshaders.mat4Uniform("model", model);

        PBRshaders.vec3Uniform("lights[0].color", lightColor1);
        PBRshaders.vec3Uniform("lights[0].pos", lightPos1);
        PBRshaders.vec3Uniform("lights[0].atten", 1.0f, 0.09f, 0.032f);
        PBRshaders.FloatUniform("lights[0].diff", 1.0f);
        PBRshaders.FloatUniform("lights[0].spec", 0.9f);

        PBRshaders.vec3Uniform("viewPos", viewPos);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, ventedTexture1);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, ventedTexture2);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, ventedTexture3);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, ventedTexture4);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, ventedTexture5);
        render.sphereRender();

        Pos = glm::vec3(currPos[0].x + 3, currPos[0].y, currPos[0].z);
        model = glm::translate(model, Pos);
        PBRshaders.mat4Uniform("model", model);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, ceramicTexture1);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, ceramicTexture2);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, ceramicTexture3);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, ceramicTexture4);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, ceramicTexture5);
        render.sphereRender();

        Pos = glm::vec3(currPos[0].x + 3, currPos[0].y, currPos[0].z);
        model = glm::translate(model, Pos);
        PBRshaders.mat4Uniform("model", model);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, beatenTexture1);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, beatenTexture2);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, beatenTexture3);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, beatenTexture4);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, beatenTexture5);
        render.sphereRender();

        // Normal

        Normalshaders.UseShaderProgramm();

        Pos = glm::vec3(currPos[0].x, currPos[0].y, currPos[0].z + 3);
        model = glm::translate(model, Pos);

        Normalshaders.mat4Uniform("proj", proj);
        Normalshaders.mat4Uniform("view", view);
        Normalshaders.mat4Uniform("model", model);

        Normalshaders.vec3Uniform("lights[0].color", lightColor1);
        Normalshaders.vec3Uniform("lights[0].pos", lightPos1);
        Normalshaders.vec3Uniform("lights[0].atten", 1.0f, 0.09f, 0.032f);
        Normalshaders.FloatUniform("lights[0].diff", 1.0f);
        Normalshaders.FloatUniform("lights[0].spec", 0.9f);

        Normalshaders.vec3Uniform("viewPos", viewPos);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, beatenTexture1);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, beatenTexture2);
        render.sphereRender();

        Pos = glm::vec3(currPos[0].x - 3, currPos[0].y, currPos[0].z);
        model = glm::translate(model, Pos);
        Normalshaders.mat4Uniform("model", model);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, ceramicTexture1);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, ceramicTexture2);
        render.sphereRender();

        Pos = glm::vec3(currPos[0].x - 3, currPos[0].y, currPos[0].z);
        model = glm::translate(model, Pos);
        Normalshaders.mat4Uniform("model", model);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, ventedTexture1);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, ventedTexture2);
        render.sphereRender();

        // Texture

        shaders.UseShaderProgramm();

        Pos = glm::vec3(currPos[0].x, currPos[0].y, currPos[0].z + 3);
        model = glm::translate(model, Pos);

        shaders.mat4Uniform("proj", proj);
        shaders.mat4Uniform("view", view);
        shaders.mat4Uniform("model", model);

        shaders.vec3Uniform("lights[0].color", lightColor1);
        shaders.vec3Uniform("lights[0].pos", lightPos1);
        shaders.vec3Uniform("lights[0].atten", 1.0f, 0.09f, 0.032f);
        shaders.FloatUniform("lights[0].diff", 1.0f);
        shaders.FloatUniform("lights[0].spec", 0.9f);

        shaders.vec3Uniform("viewPos", viewPos);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, ventedTexture1);
        render.sphereRender();

        Pos = glm::vec3(currPos[0].x + 3, currPos[0].y, currPos[0].z);
        model = glm::translate(model, Pos);
        shaders.mat4Uniform("model", model);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, ceramicTexture1);
        render.sphereRender();

        Pos = glm::vec3(currPos[0].x + 3, currPos[0].y, currPos[0].z);
        model = glm::translate(model, Pos);
        shaders.mat4Uniform("model", model);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, beatenTexture1);
        render.sphereRender();


        //Lighting render

        lightShaders.UseShaderProgramm();

        glm::mat4 lightModel1 = glm::mat4(1.0f);
        lightModel1 = glm::translate(lightModel1, lightPos1);
        lightModel1 = glm::scale(lightModel1, glm::vec3(0.05f));

        lightShaders.mat4Uniform("model", lightModel1);
        lightShaders.mat4Uniform("proj", proj);
        lightShaders.mat4Uniform("view", view);
        lightShaders.vec3Uniform("lightColor", lightColor1);

        render.sphereRender();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
