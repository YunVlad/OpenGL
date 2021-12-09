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

int process()
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
    unsigned int cruiserTexture1 = render.loadTexture("Textures/Original/cruiser/albedo.jpg");
    unsigned int cruiserTexture2 = render.loadTexture("Textures/Original/cruiser/normal.jpg");
    unsigned int cruiserTexture3 = render.loadTexture("Textures/Original/cruiser/metallic.jpg");
    unsigned int cruiserTexture4 = render.loadTexture("Textures/Original/cruiser/roughness.jpg");

    unsigned int ceramicTexture1 = render.loadTexture("Textures/Original/ceramic/albedo.jpg");
    unsigned int ceramicTexture2 = render.loadTexture("Textures/Original/ceramic/normal.jpg");
    unsigned int ceramicTexture3 = render.loadTexture("Textures/Original/ceramic/metallic.jpg");
    unsigned int ceramicTexture4 = render.loadTexture("Textures/Original/ceramic/roughness.jpg");

    unsigned int beatenTexture1 = render.loadTexture("Textures/Original/beaten/albedo.jpg");
    unsigned int beatenTexture2 = render.loadTexture("Textures/Original/beaten/normal.jpg");
    unsigned int beatenTexture3 = render.loadTexture("Textures/Original/beaten/metallic.jpg");
    unsigned int beatenTexture4 = render.loadTexture("Textures/Original/beaten/roughness.jpg");

    unsigned int paintedTexture1 = render.loadTexture("Textures/Original/painted/albedo.jpg");
    unsigned int paintedTexture2 = render.loadTexture("Textures/Original/painted/normal.jpg");
    unsigned int paintedTexture3 = render.loadTexture("Textures/Original/painted/metallic.jpg");
    unsigned int paintedTexture4 = render.loadTexture("Textures/Original/painted/roughness.jpg");


    unsigned int ResultcruiserTexture1 = render.loadTexture("Textures/Result/cruiser/albedo.jpg");
    unsigned int ResultcruiserTexture2 = render.loadTexture("Textures/Result/cruiser/normal.jpg");
    unsigned int ResultcruiserTexture3 = render.loadTexture("Textures/Result/cruiser/metallic.jpg");
    unsigned int ResultcruiserTexture4 = render.loadTexture("Textures/Result/cruiser/roughness.jpg");

    unsigned int ResultceramicTexture1 = render.loadTexture("Textures/Result/ceramic/albedo.jpg");
    unsigned int ResultceramicTexture2 = render.loadTexture("Textures/Result/ceramic/normal.jpg");
    unsigned int ResultceramicTexture3 = render.loadTexture("Textures/Result/ceramic/metallic.jpg");
    unsigned int ResultceramicTexture4 = render.loadTexture("Textures/Result/ceramic/roughness.jpg");

    unsigned int ResultbeatenTexture1 = render.loadTexture("Textures/Result/beaten/albedo.jpg");
    unsigned int ResultbeatenTexture2 = render.loadTexture("Textures/Result/beaten/normal.jpg");
    unsigned int ResultbeatenTexture3 = render.loadTexture("Textures/Result/beaten/metallic.jpg");
    unsigned int ResultbeatenTexture4 = render.loadTexture("Textures/Result/beaten/roughness.jpg");

    unsigned int ResultpaintedTexture1 = render.loadTexture("Textures/Result/painted/albedo.jpg");
    unsigned int ResultpaintedTexture2 = render.loadTexture("Textures/Result/painted/normal.jpg");
    unsigned int ResultpaintedTexture3 = render.loadTexture("Textures/Result/painted/metallic.jpg");
    unsigned int ResultpaintedTexture4 = render.loadTexture("Textures/Result/painted/roughness.jpg");

    //Shaders configure

    shader lightShaders("Shaders/VertexLight", "Shaders/ColorLight");

    shader PBRshaders("Shaders/VertexPBR", "Shaders/ColorPBR3");
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
    PBRshaders.IntUniform("normalMap", 2);
    PBRshaders.IntUniform("metallicMap", 3);
    PBRshaders.IntUniform("roughnessMap", 4);

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
        glBindTexture(GL_TEXTURE_2D, cruiserTexture1);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, cruiserTexture2);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, cruiserTexture3);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, cruiserTexture4);
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
        render.sphereRender();

        Pos = glm::vec3(currPos[0].x + 3, currPos[0].y, currPos[0].z);
        model = glm::translate(model, Pos);
        PBRshaders.mat4Uniform("model", model);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, paintedTexture1);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, paintedTexture2);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, paintedTexture3);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, paintedTexture4);
        render.sphereRender();

        //PBR-result

        Pos = glm::vec3(currPos[0].x - 9, currPos[0].y, currPos[0].z + 3);
        model = glm::translate(model, Pos);
        PBRshaders.mat4Uniform("model", model);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, ResultcruiserTexture1);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, ResultcruiserTexture2);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, ResultcruiserTexture3);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, ResultcruiserTexture4);
        render.sphereRender();

        Pos = glm::vec3(currPos[0].x + 3, currPos[0].y, currPos[0].z);
        model = glm::translate(model, Pos);
        PBRshaders.mat4Uniform("model", model);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, ResultceramicTexture1);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, ResultceramicTexture2);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, ResultceramicTexture3);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, ResultceramicTexture4);
        render.sphereRender();

        Pos = glm::vec3(currPos[0].x + 3, currPos[0].y, currPos[0].z);
        model = glm::translate(model, Pos);
        PBRshaders.mat4Uniform("model", model);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, ResultbeatenTexture1);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, ResultbeatenTexture2);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, ResultbeatenTexture3);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, ResultbeatenTexture4);
        render.sphereRender();

        Pos = glm::vec3(currPos[0].x + 3, currPos[0].y, currPos[0].z);
        model = glm::translate(model, Pos);
        PBRshaders.mat4Uniform("model", model);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, ResultpaintedTexture1);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, ResultpaintedTexture2);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, ResultpaintedTexture3);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, ResultpaintedTexture4);
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
        glBindTexture(GL_TEXTURE_2D, ResultpaintedTexture1);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, ResultpaintedTexture2);
        render.sphereRender();

        Pos = glm::vec3(currPos[0].x - 3, currPos[0].y, currPos[0].z);
        model = glm::translate(model, Pos);
        Normalshaders.mat4Uniform("model", model);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, ResultbeatenTexture1);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, ResultbeatenTexture2);
        render.sphereRender();

        Pos = glm::vec3(currPos[0].x - 3, currPos[0].y, currPos[0].z);
        model = glm::translate(model, Pos);
        Normalshaders.mat4Uniform("model", model);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, ResultceramicTexture1);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, ResultceramicTexture2);
        render.sphereRender();

        Pos = glm::vec3(currPos[0].x - 3, currPos[0].y, currPos[0].z);
        model = glm::translate(model, Pos);
        Normalshaders.mat4Uniform("model", model);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, ResultcruiserTexture1);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, ResultcruiserTexture2);
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
        glBindTexture(GL_TEXTURE_2D, ResultcruiserTexture1);
        render.sphereRender();

        Pos = glm::vec3(currPos[0].x + 3, currPos[0].y, currPos[0].z);
        model = glm::translate(model, Pos);
        shaders.mat4Uniform("model", model);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, ResultceramicTexture1);
        render.sphereRender();

        Pos = glm::vec3(currPos[0].x + 3, currPos[0].y, currPos[0].z);
        model = glm::translate(model, Pos);
        shaders.mat4Uniform("model", model);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, ResultbeatenTexture1);
        render.sphereRender();

        Pos = glm::vec3(currPos[0].x + 3, currPos[0].y, currPos[0].z);
        model = glm::translate(model, Pos);
        shaders.mat4Uniform("model", model);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, ResultpaintedTexture1);
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

int main()
{
    process();
    return 0;
}